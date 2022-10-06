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

	std::uint8_t GetPixel(const std::uint8_t* mask, const sf::Texture* tex, unsigned int x, unsigned int y)
	{
		if (x>tex->getSize().x||y>tex->getSize().y)
			return 0;

		return mask[x+y*tex->getSize().x];
	}

	std::uint8_t *GetMask(const sf::Texture* tex)
	{
		auto pair = Bitmasks.find(tex);
		if (pair == Bitmasks.end())
			return pair->second.get();

		return CreateMask(tex, tex->copyToImage());
	}

	std::uint8_t *CreateMask(const sf::Texture* tex, const sf::Image& img)
	{
		auto mask = std::make_unique<std::uint8_t[]>(tex->getSize().y * tex->getSize().x);

		for (unsigned int y = 0; y<tex->getSize().y; y++)
		{
			for (unsigned int x = 0; x<tex->getSize().x; x++)
				mask[x + y * tex->getSize().x] = img.getPixel({ x,y }).a;
		}

		auto* maskPtr = mask.get();
		Bitmasks.insert(std::pair<const sf::Texture*, std::unique_ptr<std::uint8_t[]>>(tex, std::move(mask)));

		return maskPtr;
	}
private:
	std::map<const sf::Texture*, std::unique_ptr<std::uint8_t[]>> Bitmasks;
};
	
BitmaskManager Bitmasks;
 
bool PixelPerfectTest(const sf::Sprite& Object1, const sf::Sprite& Object2, std::uint8_t AlphaLimit) 
{	
	if (const auto intersection = Object1.getGlobalBounds().findIntersection(Object2.getGlobalBounds())) 
	{
		sf::IntRect O1SubRect = Object1.getTextureRect();
		sf::IntRect O2SubRect = Object2.getTextureRect();

		std::uint8_t* mask1 = Bitmasks.GetMask(Object1.getTexture());
		std::uint8_t* mask2 = Bitmasks.GetMask(Object2.getTexture());

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
					if (Bitmasks.GetPixel(mask1, Object1.getTexture(), static_cast<int>(o1v.x) + O1SubRect.left, static_cast<int>(o1v.y) + O1SubRect.top) > AlphaLimit &&
						Bitmasks.GetPixel(mask2, Object2.getTexture(), static_cast<int>(o2v.x) + O2SubRect.left, static_cast<int>(o2v.y) + O2SubRect.top) > AlphaLimit)
					{
						return true;
					}
				}
			}
		}
	}

	return false;
}

bool CreateTextureAndBitmask(sf::Texture &LoadInto, const std::string& Filename)
{
	sf::Image img;
	if (!img.loadFromFile(Filename))
		return false;
	if (!LoadInto.loadFromImage(img))
		return false;

	Bitmasks.CreateMask(&LoadInto, img);
	return true;
}

static sf::Vector2f GetSpriteCenter(const sf::Sprite& Object)
{
	sf::FloatRect AABB = Object.getGlobalBounds();
	return sf::Vector2f (AABB.left+AABB.width/2.f, AABB.top+AABB.height/2.f);
}

static sf::Vector2f GetSpriteSize(const sf::Sprite& Object)
{
	sf::IntRect OriginalSize = Object.getTextureRect();
	sf::Vector2f Scale = Object.getScale();
	return sf::Vector2f (OriginalSize.width*Scale.x, OriginalSize.height*Scale.y);
}

bool CircleTest(const sf::Sprite& Object1, const sf::Sprite& Object2) {
	sf::Vector2f Obj1Size = GetSpriteSize(Object1);
	sf::Vector2f Obj2Size = GetSpriteSize(Object2);
	float Radius1 = (Obj1Size.x + Obj1Size.y) / 4;
	float Radius2 = (Obj2Size.x + Obj2Size.y) / 4;

	sf::Vector2f Distance = GetSpriteCenter(Object1)-GetSpriteCenter(Object2);

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
		Points[0] = transf.transformPoint({ 0.f, 0.f });
		Points[1] = transf.transformPoint({ bounds.width, 0.f });
		Points[2] = transf.transformPoint({ bounds.width, bounds.height });
		Points[3] = transf.transformPoint({ 0.f, bounds.height });
	}

	sf::Vector2f Points[4];

	MinMax<float> ProjectOntoAxis(const sf::Vector2f& Axis) // Project all four points of the OBB onto the given axis and return the dotproducts of the two outermost points
	{
		float Min = (Points[0].x*Axis.x+Points[0].y*Axis.y);
		float Max = Min;
		for (int j = 1; j<4; j++)
		{
			float Projection = (Points[j].x*Axis.x+Points[j].y*Axis.y);

			if (Projection<Min)
				Min=Projection;
			if (Projection>Max)
				Max=Projection;
		}

		return { .min = Min, .max = Max };
	}
};

bool BoundingBoxTest(sf::Transform transf1, sf::FloatRect bounds1, sf::Transform transf2, sf::FloatRect bounds2)
{
	OrientedBoundingBox OBB1(transf1, bounds1);
	OrientedBoundingBox OBB2(transf2, bounds2);

	// Create the four distinct axes that are perpendicular to the edges of the two rectangles
	sf::Vector2f Axes[4] = {
		sf::Vector2f (OBB1.Points[1].x-OBB1.Points[0].x,
		OBB1.Points[1].y-OBB1.Points[0].y),
		sf::Vector2f (OBB1.Points[1].x-OBB1.Points[2].x,
		OBB1.Points[1].y-OBB1.Points[2].y),
		sf::Vector2f (OBB2.Points[0].x-OBB2.Points[3].x,
		OBB2.Points[0].y-OBB2.Points[3].y),
		sf::Vector2f (OBB2.Points[0].x-OBB2.Points[1].x,
		OBB2.Points[0].y-OBB2.Points[1].y)
	};

	for (int i = 0; i < 4; i++) // For each axis...
	{
		// ... project the points of both OBBs onto the axis ...
		auto [MinOBB1, MaxOBB1] = OBB1.ProjectOntoAxis(Axes[i]);
		auto [MinOBB2, MaxOBB2] = OBB2.ProjectOntoAxis(Axes[i]);

		// ... and check whether the outermost projected points of both OBBs overlap.
		// If this is not the case, the Separating Axis Theorem states that there can be no collision between the rectangles
		if (!((MinOBB2<=MaxOBB1)&&(MaxOBB2>=MinOBB1)))
			return false;
	}
	return true;
}

} // sf::Collision
