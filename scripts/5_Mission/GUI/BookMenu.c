class BookMenu extends UIScriptedMenu
{
	protected TextWidget m_author;
	protected TextWidget m_title;
	protected TextWidget m_page;
	protected HtmlWidget m_content;
	protected float m_page_height;
	protected float m_content_total_height;
	protected float m_content_pos;

	override Widget Init()
	{
		layoutRoot = GetGame().GetWorkspace().CreateWidgets("gui/layouts/day_z_book.layout");
		Class.CastTo(m_content, layoutRoot.FindAnyWidget("HtmlWidget"));
		Class.CastTo(m_author, layoutRoot.FindAnyWidget("Author")); 
		Class.CastTo(m_title, layoutRoot.FindAnyWidget("Title"));
		Class.CastTo(m_page, layoutRoot.FindAnyWidget("Page"));
		
		float width;
		m_content.GetScreenSize(width, m_page_height);
		return layoutRoot;
	}
	
	void ReadBook(InventoryItem book)
	{
		m_content.LoadFile( book.ConfigGetString("file") );
		m_author.SetText( book.ConfigGetString("author") );
		m_title.SetText( book.ConfigGetString("title") );
		m_content_total_height = m_content.GetContentHeight();
		m_content_pos = 0;
		NextPrevPage(false);
	}

	override bool OnClick(Widget w, int x, int y, int button)
	{
		super.OnClick(w, x, y, button);

		switch (w.GetUserID())
		{
			case IDC_BOOK_VIEWER_PREV:
				NextPrevPage(false);
				return true;
			case IDC_BOOK_VIEWER_NEXT:
				NextPrevPage(true);
				return true;
			case IDC_CANCEL:
				Close();
				return true;
		}

		return false;
	}
	
	void NextPrevPage(bool next)
	{
		if (next)
		{
			m_content_pos	= m_content_pos + m_page_height;
		}
		else
		{
			m_content_pos	= m_content_pos - m_page_height;
		}
		
		float maxOffset = 0;
		if (m_content_total_height > m_page_height)
		{
			maxOffset = m_content_total_height - m_page_height;
		}		
		
		if (m_content_pos < 0)
		{
			m_content_pos = 0;
		}

		if (m_content_pos > maxOffset)
		{
			m_content_pos = maxOffset;
		}
		
		m_content.SetContentOffset(m_content_pos, true);
		
		float pagesTotal = Math.Ceil(m_content_total_height / m_page_height);
		float currPage = Math.Round(m_content_pos / m_page_height) + 1;

		m_page.SetText( currPage.ToString() + " / " + pagesTotal.ToString() );
	}
}
