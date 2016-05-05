#include <ctti/type_id.hpp>
#include <unordered_map>
#include <vector>

class Base {};
class A : public Base {};
class B : public Base {};

class SlotInterface
{
public:
    virtual ~SlotInterface() = default;

    virtual std::size_t size() const = 0;
    virtual const Base& at(std::size_t i) const = 0;
    virtual void push_back(const Base& object) = 0;
};

template<typename T>
class Slot : public SlotInterface
{
public:
    std::size_t size() const override
    {
        return _storage.size();
    }

    const Base& at(std::size_t i) const override
    {
        return _storage[i];
    }

    void push_back(const Base& obj) override
    {
        _storage.push_back(static_cast<const T&>(obj));
    }

private:
    std::vector<T> _storage;
};

template<typename Base>
class PolyContainer
{
public:
    std::size_t size() const
    {
        std::size_t size = 0;

        for(const auto& keyValue : _slots)
        {
            const SlotInterface& slot = *keyValue.second;
            size += slot.size();
        }

        return size;
    }

    const Base& at(std::size_t index) const
    {
        for(const auto& keyValue : _slots)
        {
            const SlotInterface& slot = *keyValue.second;

            if(index >= slot.size())
            {
                index -= slot.size();
            }
            else
            {
                return slot.at(index);
            }
        }
    }
    
    template<typename T>
    void push_back(const T& obj)
    {
        auto it = _slots.find(ctti::unnamed_type_id<T>());

        if(it == _slots.end())
        {
            _slots[ctti::unnamed_type_id<T>()] = new Slot<T>;
        }

        _slots[ctti::unnamed_type_id<T>()]->push_back(obj);
    }

private:
    std::unordered_map<ctti::type_index, std::unique_ptr<SlotInterface>> _slots;
};

