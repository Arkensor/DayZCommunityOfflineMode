class NoteMenu extends UIScriptedMenu
{
	protected MultilineEditBoxWidget m_edit;
	protected HtmlWidget m_html;
	protected EntityAI m_paper;
	protected EntityAI m_pen;
	protected string m_penColor; //color in hex-code format, transferred as string. Could be transferred as int or array<int, bool>?
	protected int m_handwriting;

	void NoteMenu()
	{
	}

	void ~NoteMenu()
	{
	}

	void InitRead(array<ref WritePaperParams> AdvancedText)//TODO add more parameters
	{
		if (m_edit)
		{
			m_edit.Show(false);
		}
		
		if (m_html)
		{
			//TODO add text formating here. Just text string for now
			//
			WritePaperParams wtp;
			string text = "";
			if (AdvancedText /*&& AdvancedText.Count() > 0*/)
			{
				for (int i = 0; i < AdvancedText.Count(); i++)
				{
					wtp = AdvancedText.Get(i);
					text += wtp.param1 + " ";
				}
			}
			//
			m_html.Show(true);
			m_html.SetText("<html><body><p>" + text + "</p></body></html>");
		}
	}

	void InitWrite(EntityAI paper, EntityAI pen, int handwriting)
	{
		if (m_edit)
		{
			//string text;
			m_edit.Show(true);
			m_edit.SetText("");
			
			m_paper = paper;
			m_pen = pen;
			m_penColor = m_pen.ConfigGetString("writingColor");
			if (!m_penColor)
			{
				m_penColor = "#000000";
			}
			/*penString = penString.Substring(1,penString.Length() - 1); //omits the "#"
			m_penColor = penString.ToInt();*/
			m_handwriting = handwriting;
		}
		
		if (m_html)
		{
			m_html.Show(false);
		}
	}

	override Widget Init()
	{
		layoutRoot = GetGame().GetWorkspace().CreateWidgets("gui/layouts/day_z_inventory_note.layout");
		m_edit = MultilineEditBoxWidget.Cast( layoutRoot.FindAnyWidget("EditWidget") );
		m_html = HtmlWidget.Cast( layoutRoot.FindAnyWidget("HtmlWidget") );
		
		return layoutRoot;
	}

	override bool OnClick(Widget w, int x, int y, int button)
	{
		super.OnClick(w, x, y, button);
		
		switch (w.GetUserID())
		{
			case IDC_CANCEL:
				Close();
				return true;
			case IDC_OK:
				if (m_paper && m_pen)
				{
					WritePaperParams params = new WritePaperParams("", m_penColor,m_handwriting);
					m_edit.GetText(params.param1);
					m_paper.RPCSingleParam(ERPCs.RPC_WRITE_NOTE, params, true);
				}
				Close();
			return true;
		}

		return false;
	}
}
