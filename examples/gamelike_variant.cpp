
#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>
#include <cassert>
#include <random>

#include "variant/variant.hpp"
#include "variant/multi_visitor.hpp"

struct Particle
{
    Particle(float x, float y) :
        x{x},
        y{y}
    {}

    float x, y;

	friend std::ostream& operator<<(std::ostream& os, const Particle& particle)
	{
		return os << "(" << particle.x << ", " << particle.y << ")";
	}
};

struct AABB
{
    AABB(float x, float y, float width, float height) :
        x{x},
        y{y},
        width{width},
        height{height}
    {}

    float x, y, width, height;

    float left() const
    {
        return x;
    }

    float right() const
    {
        return x + width;
    }

    float top() const
    {
        return y;
    }

    float bottom() const
    {
        return y + height;
    }

	friend std::ostream& operator<<(std::ostream& os, const AABB& aabb)
	{
		return os << "[" << aabb.x << ", " << aabb.y << ", " << aabb.width << ", " << aabb.height << "]";
	}
};

struct Collider
{
    static bool collide(const AABB& box, const Particle& p)
    {
        return p.x >= box.left() && p.x <= box.right() &&
               p.y >= box.bottom() && p.y <= box.top();
    }

    static bool collide(const Particle& p, const AABB& box)
    {
        return collide(box, p);
    }

    static bool collide(const AABB& a, const AABB& b)
    {
        return collide(AABB{a.x, a.y, a.width + b.width, a.height + b.height},
                       Particle{b.x, b.y});
    }

	template<typename Lhs, typename Rhs>
	static constexpr bool collide(const Lhs&, const Rhs&)
	{
		return false;
	}
};

class MyCoolGameEngine
{
public:
    MyCoolGameEngine(std::size_t count)
    {
        std::default_random_engine prng{std::random_device()()};
        std::uniform_real_distribution<float> dist{-1.0f, 1.0f};

        for(std::size_t i = 0; i < count; ++i)
        {
            float x = dist(prng);
            float y = dist(prng);

            if(dist(prng) >= 0.0f)
                _entities.emplace_back(Particle{x, y});
            else
            {
                float width = std::abs(dist(prng));
                float height = std::abs(dist(prng));

                _entities.emplace_back(AABB{x, y, width, height});
            }
        }
    }

    void doPhysics() const
    {
        for(const auto& a : _entities)
        {
            for(const auto& b : _entities)
            {
                bool collide = cpp::multi_visitor<bool>(
                    [](const auto& lhs, const auto& rhs)
                    {
                        return Collider::collide(lhs, rhs);
                    }
                )(a, b);

				std::cout << a << " vs " << b << ": " << std::boolalpha << collide << std::endl;
            }
        }
    }

private:
    std::vector<cpp::Variant<Particle, AABB>> _entities;
};

int main()
{
    MyCoolGameEngine engine{10};

    engine.doPhysics();
}
