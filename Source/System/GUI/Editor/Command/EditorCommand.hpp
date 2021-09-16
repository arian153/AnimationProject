#pragma once
#include <string>
#include <vcruntime_typeinfo.h>

namespace CS460
{
    class CommandRegistry;

    class EditorCommand
    {
    public:
        EditorCommand();
        virtual ~EditorCommand();

        virtual void Execute() = 0;
        virtual void UnExecute() = 0;
        void         SetRegistry(CommandRegistry* registry);
        std::string  Type() const;
        const char*  TypeCStr() const;

    protected:
        CommandRegistry* m_registry = nullptr;
        std::string      m_type;
    private:
    };

    template <typename T>
    class EditData final : public EditorCommand
    {
    public:
        EditData(T& data, const T& changed)
            : m_data(data), m_prev(data), m_next(changed)
        {
            m_type = "Edit Data : ";
            m_type += typeid(T).name();
        }

        ~EditData()
        {
        }

        void Execute() override
        {
            m_data = m_next;
        }

        void UnExecute() override
        {
            m_data = m_prev;
        }

    private:
        T& m_data;
        T  m_prev;
        T  m_next;
    };

    template <typename T, typename I, void(I::*F)(const T&)>
    class EditFunction final : public EditorCommand
    {
    private:
        using Function = void(*)(void*, const T&);
    public:
        EditFunction(void* instance, const T& prev, const T& next)
            : m_instance(instance), m_prev(prev), m_next(next)
        {
            m_function = &MemberFunction<I, F>;
            m_type = "Edit Data : ";
            m_type += typeid(T).name();
        }

        ~EditFunction()
        {
        }

        void Execute() override
        {
            m_function(m_instance, m_next);
        }

        void UnExecute() override
        {
            m_function(m_instance, m_prev);
        }

    private: //template
        template <typename Inst, void(Inst::*Func)(const T&)>
        static void MemberFunction(void* instance, const T& data)
        {
            (static_cast<Inst*>(instance)->*Func)(data);
        }

    private:
        void*    m_instance;
        Function m_function;
        T        m_prev;
        T        m_next;
    };
}
