#pragma once

class Object;
class ObjectFactory
{
public:
	virtual ~ObjectFactory() = default;
	virtual Object *create() = 0;
};

class Object
{
public:
	using FactoryMap = std::map<std::string, ObjectFactory*>;

	Object() = default;
	virtual ~Object() = default;

	static void registerType(const std::string& name, ObjectFactory *factory)
	{
		getMap().try_emplace(name, factory);
	}

	static bool isRegistered(const std::string &name)
	{
		return getMap().find(name) != getMap().end();
	}

	static Object *create(const std::string &name)
	{
		auto it = getMap().find(name);
		if (it == getMap().end())
			throw;

		return it->second->create();
	}

private:
	static FactoryMap &getMap()
	{
		static FactoryMap factories;
		return factories;
	}
};

#define REGISTER_TYPE(klass) \
    class klass##Factory : public ObjectFactory { \
    public: \
        klass##Factory() \
        { \
            Object::registerType(#klass, this); \
        } \
        Object *create() override { \
            return new klass; \
        } \
    }; \
    static klass##Factory global_##klass##Factory;
