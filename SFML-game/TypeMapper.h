#pragma once

// TODO: Replace this by not using macros

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
	Object() = default;
	virtual ~Object() = default;

	static bool isRegistered(const std::string &name)
	{
		return getMap().find(name) != getMap().end();
	}

	// Used by macro MAKE_INSTANCE
	template <class T>
	static T *create(const std::string &name)
	{
		if (!isRegistered(name))
			return nullptr;

		auto obj = create(name);

		auto castedObj = dynamic_cast<T*>(obj);
		if (castedObj)
			return castedObj;

		delete obj;
		return nullptr;
	}

	// Required by macro REGISTER_TYPE
	static void registerType(const std::string& name, ObjectFactory *factory)
	{
		getMap().try_emplace(name, factory);
	}

private:
	using FactoryMap = std::map<std::string, ObjectFactory*>;

	static Object *create(const std::string &name)
	{
		auto it = getMap().find(name);
		if (it == getMap().end())
			throw;

		return it->second->create();
	}

	static FactoryMap &getMap()
	{
		static FactoryMap factories;
		return factories;
	}
};

#define MAKE_INSTANCE(val, D) Object::create<D>(##val)
#define REGISTER_TYPE(klass) \
    class klass##Factory : public ObjectFactory { \
    public: \
        klass##Factory() \
        { \
            Object::registerType(#klass, this); \
        } \
        Object *create() override \
		{ \
            return new klass; \
        } \
    }; \
    static klass##Factory global_##klass##Factory;
