class KeyMouseBinding
{
    protected typename m_Object;
    protected ref array< string > m_KeyBinds;

    protected string m_CallbackFunction;
    protected string m_UAInputName;
    protected string m_Description;

    protected bool m_CanBeUsedInMenu;

    protected int m_ActionType;
    
    void KeyMouseBinding( typename object, string callback, string description, bool menu = false ) 
    {
        m_Object = object;
        m_KeyBinds = new array< string >;
        
        m_CallbackFunction = callback;
        m_UAInputName = "UA" + object.ToString() + callback;

        m_Description = description;
        
        m_CanBeUsedInMenu = menu;

        m_ActionType = KeyMouseActionType.PRESS;
    }
    
    bool CanBeUsedInMenu() 
    {
        return m_CanBeUsedInMenu;
    }
    
    void AddBinding( string key, int action = 1)
    {
        m_KeyBinds.Insert( key );
        m_ActionType = action;
    }
    
    ref array< string > GetBindings() 
    {
        return m_KeyBinds;
    }

    void SetActionType( int type ) 
    {
        m_ActionType = type;
    }

    int GetActionType()
    {
        return m_ActionType;
    }
    
    typename GetObject() 
    {
        return m_Object;
    }
    
    string GetCallBackFunction() 
    {
        return m_CallbackFunction;
    }

    string GetUAInputName()
    {
        return m_UAInputName;
    }

    string GetDescription()
    {
        return m_Description;
    }
}