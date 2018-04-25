// #include "Scripts\Classes\Gui\ChatLine.c"

class Chat
{
	const int LINE_COUNT = 8;

	private Widget m_root_widget;
	private int m_line_height;
	private int m_last_line;
	private ref array<ref ChatLine> m_lines;

	void Chat()
	{
		m_lines = new array<ref ChatLine>;
	}

	void ~Chat()
	{
		Destroy();
	}
	
	void Init(Widget root_widget)
	{
		Destroy();

		m_root_widget = root_widget;

		if (m_root_widget)
		{
			float w;
			float h;
			m_root_widget.GetSize(w,h);
			m_line_height = h / LINE_COUNT;
			m_last_line = 0;

			for (int i = 0; i < LINE_COUNT; i++)
			{
				ChatLine line = new ChatLine(m_root_widget);
				m_lines.Insert(line);
			}
		}
	}

	void Destroy()
	{
		m_lines.Clear();
	}
	
	void Clear()
	{
		for (int i = 0; i < LINE_COUNT; i++)
		{
			m_lines.Get(i).Clear();
		}
	}
	
	void Add(ChatMessageEventParams params)
	{
		int max_lenght = 60;
		int name_lenght = params.param2.Length();
		int text_lenght = params.param3.Length();
		int total_lenght = text_lenght + name_lenght;

		if (total_lenght > max_lenght)
		{
			int pos = 0;
			int lenght = Math.Clamp(max_lenght - name_lenght, 0, text_lenght);
			ref ChatMessageEventParams tmp = new ChatMessageEventParams(params.param1, params.param2, "", params.param4);
			
			while (pos < text_lenght)
			{
				tmp.param3 = params.param3.Substring(pos, lenght);
				AddInternal(tmp);
				
				tmp.param2 = "";
				pos += lenght;
				lenght = Math.Clamp(text_lenght - pos, 0, max_lenght);			
			}
		}
		else
		{
			AddInternal(params);
		}
	}
	
	void AddInternal(ChatMessageEventParams params)
	{
		m_last_line = (m_last_line + 1) % m_lines.Count();

		ChatLine line = m_lines.Get(m_last_line);
		line.Set(params);

		for (int i = 0; i < m_lines.Count(); i++)
		{
			line = m_lines.Get((m_last_line + 1 + i) % LINE_COUNT);
			line.m_text_widget.SetPos(0, i * m_line_height);
			
			
			float x = 0;
			float y = 0;
			
			line.m_text_widget.GetPos(x, y);
			x = x + 1;
			y = y + 1;
			line.m_text_widget_bg.SetPos(x, y);
		}
	}
}
