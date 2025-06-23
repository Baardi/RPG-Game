/* 
 * File:   collision.cpp
 * Author: Nick (original version), ahnonay (SFML2 compatibility)
 * @@ NOTE: Modified by Bård Sigurd Møller, to fit my needs
 */

#include "stdafx.h"
#include "SFML/Collision.hpp"

namespace sf::collision {

class BitmaskManager
{
public:

	std::uint8_t getPixel(const std::vector<std::uint8_t> &mask, const sf::Texture &texture, unsigned int x, unsigned int y)
	{
		auto [width, height] = texture.getSize();
		if (x > width || y > height)
			return 0;

		return mask[x + y * width];
	}

	std::vector<std::uint8_t> &getMask(const sf::Texture &texture)
	{
		auto pair = m_bitMasks.find(&texture);
		if (pair == m_bitMasks.end())
			return pair->second;

		return createMask(texture, texture.copyToImage());
	}

	std::vector<std::uint8_t> &createMask(const sf::Texture &texture, const sf::Image &img)
	{
		auto [width, height] = texture.getSize();
		auto [maskIter, inserted] = m_bitMasks.emplace(&texture, std::vector<std::uint8_t>(width * height));
		auto &mask = maskIter->second;

		for (unsigned int y = 0; y < height; y++)
		{
			for (unsigned int x = 0; x < width; x++)
				mask[x + y * width] = img.getPixel({ x,y }).a;
		}

		return mask;
	}
private:
	std::unordered_map<const sf::Texture*, std::vector<std::uint8_t>> m_bitMasks;
};
	
static BitmaskManager s_bitmasks;
 
bool pixelPerfectTest(const sf::Sprite &object1, const sf::Sprite &object2, std::uint8_t alphaLimit) 
{	
	if (const auto intersection = object1.getGlobalBounds().findIntersection(object2.getGlobalBounds())) 
	{
		sf::IntRect O1SubRect = object1.getTextureRect();
		sf::IntRect O2SubRect = object2.getTextureRect();

		std::vector<std::uint8_t> &mask1 = s_bitmasks.getMask(object1.getTexture());
		std::vector<std::uint8_t> &mask2 = s_bitmasks.getMask(object2.getTexture());

		// Loop through our pixels
		for (int i = static_cast<int>(intersection->position.x); i < static_cast<int>(intersection->position.x + intersection->size.x); i++) 
		{
			for (int j = static_cast<int>(intersection->position.y); j < static_cast<int>(intersection->position.y + intersection->size.y); j++) 
			{ 
				sf::Vector2f o1v = object1.getInverseTransform().transformPoint({ static_cast<float>(i), static_cast<float>(j) });
				sf::Vector2f o2v = object2.getInverseTransform().transformPoint({ static_cast<float>(i), static_cast<float>(j) });
 
				// Make sure pixels fall within the sprite's subrect
				if (o1v.x > 0 && o1v.y > 0 && o2v.x > 0 && o2v.y > 0 &&
					o1v.x < O1SubRect.size.x && o1v.y < O1SubRect.size.y &&
					o2v.x < O2SubRect.size.x && o2v.y < O2SubRect.size.y) 
				{
					if (s_bitmasks.getPixel(mask1, object1.getTexture(), static_cast<int>(o1v.x) + O1SubRect.position.x, static_cast<int>(o1v.y) + O1SubRect.position.y) > alphaLimit &&
						s_bitmasks.getPixel(mask2, object2.getTexture(), static_cast<int>(o2v.x) + O2SubRect.position.x, static_cast<int>(o2v.y) + O2SubRect.position.y) > alphaLimit)
					{
						return true;
					}
				}
			}
		}
	}

	return false;
}

bool createTextureAndBitmask(sf::Texture &targetTexture, const std::filesystem::path &filename)
{
	sf::Image image;
	if (!image.loadFromFile(filename))
		return false;

	if (!targetTexture.loadFromImage(image))
		return false;

	s_bitmasks.createMask(targetTexture, image);
	return true;
}

static sf::Vector2f getSpriteCenter(const sf::Sprite &object)
{
	sf::FloatRect AABB = object.getGlobalBounds();
	
	return sf::Vector2f (AABB.position.x+AABB.size.x/2.f, AABB.position.y+AABB.size.y/2.f);
}

static sf::Vector2f getSpriteSize(const sf::Sprite &object)
{
	sf::IntRect originalSize = object.getTextureRect();
	sf::Vector2f scale = object.getScale();

	return sf::Vector2f(originalSize.size.x * scale.x, originalSize.size.y * scale.y);
}

bool circleTest(const sf::Sprite &object1, const sf::Sprite &object2) 
{
	sf::Vector2f obj1Size = getSpriteSize(object1);
	sf::Vector2f obj2Size = getSpriteSize(object2);
	float radius1 = (obj1Size.x + obj1Size.y) / 4;
	float radius2 = (obj2Size.x + obj2Size.y) / 4;

	sf::Vector2f distance = getSpriteCenter(object1) - getSpriteCenter(object2);

	return (distance.x * distance.x + distance.y * distance.y <= (radius1 + radius2) * (radius1 + radius2));
}

template<class T>
struct MinMax
{
	T min;
	T max;
};

class OrientedBoundingBox // Used in the BoundingBoxTest
{
	sf::Vector2f m_points[4];
public:
	OrientedBoundingBox(sf::Transform transf, sf::FloatRect bounds) // Calculate the four points of the OBB from a transformed (scaled, rotated...) sprite
	{
		m_points[0] = transf.transformPoint({ 0.f, 0.f });
		m_points[1] = transf.transformPoint({ bounds.size.x, 0.f });
		m_points[2] = transf.transformPoint({ bounds.size.x, bounds.size.y });
		m_points[3] = transf.transformPoint({ 0.f, bounds.size.y });
	}

	template <typename Self>
	auto& operator[](this Self &&self, size_t index) 
	{
		return self.m_points[index];
	}

	MinMax<float> projectOntoAxis(const sf::Vector2f &axis) // Project all four points of the OBB onto the given axis and return the dotproducts of the two outermost points
	{
		float min = m_points[0].x * axis.x + m_points[0].y * axis.y;
		float max = min;

		for (int j = 1; j<4; j++)
		{
			float Projection = (m_points[j].x * axis.x + m_points[j].y * axis.y);

			if (Projection < min)
				min = Projection;
			if (Projection > max)
				max = Projection;
		}

		return { .min = min, .max = max };
	}
};

bool boundingBoxTest(sf::Transform transf1, sf::FloatRect bounds1, sf::Transform transf2, sf::FloatRect bounds2)
{
	OrientedBoundingBox OBB1(transf1, bounds1);
	OrientedBoundingBox OBB2(transf2, bounds2);

	// Create the four distinct axes that are perpendicular to the edges of the two rectangles
	std::array<sf::Vector2f, 4> axes
	{
		sf::Vector2f (OBB1[1].x-OBB1[0].x, OBB1[1].y-OBB1[0].y),
		sf::Vector2f (OBB1[1].x-OBB1[2].x, OBB1[1].y-OBB1[2].y),
		sf::Vector2f (OBB2[0].x-OBB2[3].x, OBB2[0].y-OBB2[3].y),
		sf::Vector2f (OBB2[0].x-OBB2[1].x, OBB2[0].y-OBB2[1].y)
	};

	for (int i = 0; i < 4; i++) // For each axis...
	{
		// ... project the points of both OBBs onto the axis ...
		auto [minOBB1, maxOBB1] = OBB1.projectOntoAxis(axes[i]);
		auto [minOBB2, maxOBB2] = OBB2.projectOntoAxis(axes[i]);

		// ... and check whether the outermost projected points of both OBBs overlap.
		// If this is not the case, the Separating Axis Theorem states that there can be no collision between the rectangles
		if (!((minOBB2 <= maxOBB1) && (maxOBB2 >= minOBB1)))
			return false;
	}
	return true;
}

} // sf::Collision
