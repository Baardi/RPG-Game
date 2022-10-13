/* 
 * File:   collision.cpp
 * Author: Nick (original version), ahnonay (SFML2 compatibility)
 * @@ NOTE: Modified by Bård Sigurd Møller, to fit my needs
 */

#include "stdafx.h"
#include "SFML/Collision.hpp"

namespace sf::Collision {

class BitmaskManager
{
public:

	std::uint8_t getPixel(const std::uint8_t* mask, const sf::Texture* tex, unsigned int x, unsigned int y)
	{
		auto [width, height] = tex->getSize();
		if (x > width || y > height)
			return 0;

		return mask[x + y * width];
	}

	std::uint8_t *getMask(const sf::Texture* tex)
	{
		auto pair = m_bitMasks.find(tex);
		if (pair == m_bitMasks.end())
			return pair->second.get();

		return createMask(tex, tex->copyToImage());
	}

	std::uint8_t *createMask(const sf::Texture* tex, const sf::Image& img)
	{
		auto [width, height] = tex->getSize();
		auto [maskIter, inserted] = m_bitMasks.emplace(tex, std::make_unique<std::uint8_t[]>(width * height));
		auto *mask = maskIter->second.get();

		for (unsigned int y = 0; y < height; y++)
		{
			for (unsigned int x = 0; x < width; x++)
				mask[x + y * width] = img.getPixel({ x,y }).a;
		}

		return mask;
	}
private:
	std::map<const sf::Texture*, std::unique_ptr<std::uint8_t[]>> m_bitMasks;
};
	
static BitmaskManager s_bitmasks;
 
bool pixelPerfectTest(const sf::Sprite& Object1, const sf::Sprite& Object2, std::uint8_t AlphaLimit) 
{	
	if (const auto intersection = Object1.getGlobalBounds().findIntersection(Object2.getGlobalBounds())) 
	{
		sf::IntRect O1SubRect = Object1.getTextureRect();
		sf::IntRect O2SubRect = Object2.getTextureRect();

		std::uint8_t* mask1 = s_bitmasks.getMask(Object1.getTexture());
		std::uint8_t* mask2 = s_bitmasks.getMask(Object2.getTexture());

		// Loop through our pixels
		for (int i = static_cast<int>(intersection->left); i < static_cast<int>(intersection->left + intersection->width); i++) 
		{
			for (int j = static_cast<int>(intersection->top); j < static_cast<int>(intersection->top + intersection->height); j++) 
			{ 
				sf::Vector2f o1v = Object1.getInverseTransform().transformPoint({ static_cast<float>(i), static_cast<float>(j) });
				sf::Vector2f o2v = Object2.getInverseTransform().transformPoint({ static_cast<float>(i), static_cast<float>(j) });
 
				// Make sure pixels fall within the sprite's subrect
				if (o1v.x > 0 && o1v.y > 0 && o2v.x > 0 && o2v.y > 0 &&
					o1v.x < O1SubRect.width && o1v.y < O1SubRect.height &&
					o2v.x < O2SubRect.width && o2v.y < O2SubRect.height) 
				{
					if (s_bitmasks.getPixel(mask1, Object1.getTexture(), static_cast<int>(o1v.x) + O1SubRect.left, static_cast<int>(o1v.y) + O1SubRect.top) > AlphaLimit &&
						s_bitmasks.getPixel(mask2, Object2.getTexture(), static_cast<int>(o2v.x) + O2SubRect.left, static_cast<int>(o2v.y) + O2SubRect.top) > AlphaLimit)
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
	sf::Image img;
	if (!img.loadFromFile(filename))
		return false;

	if (!targetTexture.loadFromImage(img))
		return false;

	s_bitmasks.createMask(&targetTexture, img);
	return true;
}

static sf::Vector2f getSpriteCenter(const sf::Sprite& Object)
{
	sf::FloatRect AABB = Object.getGlobalBounds();
	
	return sf::Vector2f (AABB.left+AABB.width/2.f, AABB.top+AABB.height/2.f);
}

static sf::Vector2f getSpriteSize(const sf::Sprite& Object)
{
	sf::IntRect OriginalSize = Object.getTextureRect();
	sf::Vector2f Scale = Object.getScale();

	return sf::Vector2f (OriginalSize.width*Scale.x, OriginalSize.height*Scale.y);
}

bool circleTest(const sf::Sprite& Object1, const sf::Sprite& Object2) 
{
	sf::Vector2f Obj1Size = getSpriteSize(Object1);
	sf::Vector2f Obj2Size = getSpriteSize(Object2);
	float Radius1 = (Obj1Size.x + Obj1Size.y) / 4;
	float Radius2 = (Obj2Size.x + Obj2Size.y) / 4;

	sf::Vector2f Distance = getSpriteCenter(Object1) - getSpriteCenter(Object2);

	return (Distance.x * Distance.x + Distance.y * Distance.y <= (Radius1 + Radius2) * (Radius1 + Radius2));
}

template<class T>
struct MinMax
{
	T min;
	T max;
};

class OrientedBoundingBox // Used in the BoundingBoxTest
{
public:
	OrientedBoundingBox(sf::Transform transf, sf::FloatRect bounds) // Calculate the four points of the OBB from a transformed (scaled, rotated...) sprite
	{
		points[0] = transf.transformPoint({ 0.f, 0.f });
		points[1] = transf.transformPoint({ bounds.width, 0.f });
		points[2] = transf.transformPoint({ bounds.width, bounds.height });
		points[3] = transf.transformPoint({ 0.f, bounds.height });
	}

	sf::Vector2f points[4];

	MinMax<float> ProjectOntoAxis(const sf::Vector2f& Axis) // Project all four points of the OBB onto the given axis and return the dotproducts of the two outermost points
	{
		float min = points[0].x * Axis.x + points[0].y * Axis.y;
		float max = min;

		for (int j = 1; j<4; j++)
		{
			float Projection = (points[j].x * Axis.x + points[j].y * Axis.y);

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
	sf::Vector2f Axes[4] = {
		sf::Vector2f (OBB1.points[1].x-OBB1.points[0].x,
		OBB1.points[1].y-OBB1.points[0].y),
		sf::Vector2f (OBB1.points[1].x-OBB1.points[2].x,
		OBB1.points[1].y-OBB1.points[2].y),
		sf::Vector2f (OBB2.points[0].x-OBB2.points[3].x,
		OBB2.points[0].y-OBB2.points[3].y),
		sf::Vector2f (OBB2.points[0].x-OBB2.points[1].x,
		OBB2.points[0].y-OBB2.points[1].y)
	};

	for (int i = 0; i < 4; i++) // For each axis...
	{
		// ... project the points of both OBBs onto the axis ...
		auto [minOBB1, maxOBB1] = OBB1.ProjectOntoAxis(Axes[i]);
		auto [minOBB2, maxOBB2] = OBB2.ProjectOntoAxis(Axes[i]);

		// ... and check whether the outermost projected points of both OBBs overlap.
		// If this is not the case, the Separating Axis Theorem states that there can be no collision between the rectangles
		if (!((minOBB2 <= maxOBB1) && (maxOBB2 >= minOBB1)))
			return false;
	}
	return true;
}

} // sf::Collision
