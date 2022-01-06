#include "head.h"

/***************************************************************************
  �������ƣ�_insert
  ��    �ܣ�����ģʽ���뺯��
***************************************************************************/
int Editor::_insert(char ch) {
	GetConsoleScreenBufferInfo(hOutput, &csbi);
	pos.X = csbi.dwCursorPosition.X;
	pos.Y = csbi.dwCursorPosition.Y;
	string str = GetText(pos.X, pos.Y);
	int i = pos.Y, j = pos.X;
	int k = 0;
	text[i][j] = ch;
	j++;
	for (; j < 80; j++) {
		text[i][j] = str[k];
		k++;
	}
	i++;
	for (; j < 80; j++)
		for (i = 0; i < 50; i++) {
			text[i][j] = str[k];
			k++;
		}

	history.push(_operator{ GetText(0,0),pos.X,pos.Y });
	Goto(pos.X, pos.Y);
	cout << ch;
	cout << str.substr(0, 3999 - pos.X - pos.Y * 80);
	Goto(pos.X, pos.Y);
	return 0;
}

/***************************************************************************
  �������ƣ�cct_setcursor
  ��    �ܣ����ù��״̬����ʾ/����ʾ/ȫ��/���/���ߵȣ�
***************************************************************************/
int Editor::setcursor(const int options)
{
	CONSOLE_CURSOR_INFO cursor_info;
	BOOL CreateCaret(HWND hWnd, HBITMAP hBitmap, int nWidth, int nHeight);
	BOOL ShowCaret(HWND hWnd);

	switch (options) {
	case CURSOR_VISIBLE_FULL:
		cursor_info.bVisible = 1;
		cursor_info.dwSize = 100;
		break;
	case CURSOR_VISIBLE_HALF:
		cursor_info.bVisible = 1;
		cursor_info.dwSize = 50;
		break;
	case CURSOR_INVISIBLE:
		cursor_info.bVisible = 0;
		cursor_info.dwSize = 1;
		break;
	case CURSOR_VISIBLE_NORMAL:
	default: //ȱʡ��ʾ��꣬����
		cursor_info.bVisible = 1;
		cursor_info.dwSize = 25;
		break;
	}
	SetConsoleCursorInfo(hOutput, &cursor_info);
	return 0;
}

/***************************************************************************
  �������ƣ�Del
  ��    �ܣ�ɾ��
***************************************************************************/
int  Editor::Del(COORD pos) {
	int i, j, k;
	string str =GetText(pos.X+1, pos.Y);
	cout<<str;
	Goto(pos.X, pos.Y);
	k = 0; 
	i = pos.Y;
	for (j = pos.X; j < 80; j++, k++) text[i][j] = str[k];
	for (i++; i < 50; i++)
		for (j = 0; j < 80; j++, k++)
			text[i][j] = str[k];
	history.push({GetText(0,0),pos.X,pos.Y});
	return 0;
}

/***************************************************************************
  �������ƣ�Cancel
  ��    �ܣ�����
***************************************************************************/
int Editor::Cancel() {
	GetConsoleScreenBufferInfo(hOutput, &csbi);
	if (!history.empty()) {
		Goto(0, 0);
		cout<< history.top().str;
		pos.X = history.top().x;
		pos.Y = history.top().y;
		Goto(history.top().x, history.top().y);
		history.pop();
	}
	return 0;
}

/***************************************************************************
  �������ƣ�cct_setconsoleborder
  ��    �ܣ����ô��ںͻ�������С
***************************************************************************/
int Editor::cct_setconsoleborder(int set_cols, int set_lines, int set_buffer_cols, int set_buffer_lines)
{
	/* ȡ��ǰϵͳ����Ĵ��ڵ��������ֵ */
	COORD max_coord;
	max_coord = GetLargestConsoleWindowSize(hOutput); /* .X �� .Y �ֱ��Ǵ��ڵ��к��е����ֵ */

	/* �������ô��ڵ����еķǷ�ֵ */
	if (set_cols <= 0 || set_lines <= 0)
		return 0;
	if (set_cols > max_coord.X)
		set_cols = max_coord.X;
	if (set_lines > max_coord.Y)
		set_lines = max_coord.Y;

	/* ���ô��ڵ����д�С����0��ʼ��0 ~ lines-1, 0 ~ cols-1��*/
	SMALL_RECT rect;
	rect.Top = 0;
	rect.Bottom = set_lines - 1;
	rect.Left = 0;
	rect.Right = set_cols - 1;

	/* ���û����������д�С(ȱʡ��С�ڴ���ֵ���봰��ֵһ��) */
	COORD cr;
	cr.X = (set_buffer_cols == -1 || set_buffer_cols < set_cols) ? set_cols : set_buffer_cols;		//δ�����������ֵС��set_cols����set_cols��δ��������
	cr.Y = (set_buffer_lines == -1 || set_buffer_lines < set_lines) ? set_lines : set_buffer_lines;	//δ�����������ֵС��set_lines����set_lines��δ��������

	/* ȡ��ǰ���ڼ��������Ĵ�С(����getconsoleborder) */
	int cur_cols, cur_lines, cur_buffer_cols, cur_buffer_lines;
	CONSOLE_SCREEN_BUFFER_INFO binfo;
	GetConsoleScreenBufferInfo(hOutput, &binfo);

	cur_cols = binfo.srWindow.Right - binfo.srWindow.Left + 1;	//�ɼ����ڵ�����
	cur_lines = binfo.srWindow.Bottom - binfo.srWindow.Top + 1;	//�ɼ����ڵ�����
	cur_buffer_cols = binfo.dwSize.X;							//������������
	cur_buffer_lines = binfo.dwSize.Y;							//������������

	cct_cls();
	/* ����˳��(��֤���ô��ڴ�Сʱ���ֻ���������ֵ>����ֵ) */
	if (cr.X <= cur_buffer_cols) {
		if (cr.Y <= cur_buffer_lines) {
			SetConsoleWindowInfo(hOutput, true, &rect);//���ô���
			SetConsoleScreenBufferSize(hOutput, cr);//���û�����
		}
		else { //cr.Y > cur_buffer_lines����Ҫ�û��������������
			COORD tmpcr;
			tmpcr.X = cur_buffer_cols;
			tmpcr.Y = cr.Y;
			SetConsoleScreenBufferSize(hOutput, tmpcr);//���û�����

			SetConsoleWindowInfo(hOutput, true, &rect);//���ô���
			SetConsoleScreenBufferSize(hOutput, cr);//���û�����
		}
	}
	else {//cr.X > cur_buffer_cols
		if (cr.Y >= cur_buffer_lines) {
			SetConsoleScreenBufferSize(hOutput, cr);//���û�����
			SetConsoleWindowInfo(hOutput, true, &rect);//���ô���
		}
		else {	//cr.Y < cur_buffer_lines
			COORD tmpcr;
			tmpcr.X = cr.X;
			tmpcr.Y = cur_buffer_lines;
			SetConsoleScreenBufferSize(hOutput, tmpcr);//���û�����

			SetConsoleWindowInfo(hOutput, true, &rect);//���ô���
			SetConsoleScreenBufferSize(hOutput, cr);//���û�����
		}
	}
	return 0;
}
int Editor::cct_cls()
{
	COORD coord = { 0, 0 };
	CONSOLE_SCREEN_BUFFER_INFO binfo; /* to get buffer info */
	DWORD num;

	/* ȡ��ǰ��������Ϣ */
	GetConsoleScreenBufferInfo(hOutput, &binfo);
	/* ����ַ� */
	FillConsoleOutputCharacter(hOutput, (TCHAR)' ', binfo.dwSize.X * binfo.dwSize.Y, coord, &num);
	/* ������� */
	FillConsoleOutputAttribute(hOutput, binfo.wAttributes, binfo.dwSize.X * binfo.dwSize.Y, coord, &num);

	/* ���ص�(0,0) */
	SetConsoleCursorPosition(hOutput, coord);
	return 0;
}
/***************************************************************************
  �������ƣ�GetPos
  ��    �ܣ�ȡ��ǰ�������λ�õ�����ֵ
***************************************************************************/
int Editor::GetPos(int& x, int& y)
{
    CONSOLE_SCREEN_BUFFER_INFO binfo;
    GetConsoleScreenBufferInfo(hOutput, &binfo);
    x = binfo.dwCursorPosition.X;
    y = binfo.dwCursorPosition.Y;
    return 0;
}
/***************************************************************************
  �������ƣ�Goto
  ��    �ܣ���λ����
***************************************************************************/
int Editor::Goto(const int X, const int Y)
{
    COORD coord;
    coord.X = X;
    coord.Y = Y;
    SetConsoleCursorPosition(hOutput, coord);
    return 0;
}
/***************************************************************************
  �������ƣ�StoreFile
  ��    �ܣ������ı����ļ�
***************************************************************************/
int Editor::StoreFile(string address) {
    ofstream outFile;
    outFile.open(address);
    string AllString = GetText(0, 0);
    int AllString_Size = AllString.size();
    for (int i = 0; i < AllString_Size; i++)
    {
        outFile << AllString[i];
    }
    outFile.close();
    return 0;
};
/***************************************************************************
  �������ƣ�OpenFile
  ��    �ܣ����ļ�
***************************************************************************/
int Editor::OpenFile(string address) {
    ifstream in;
    in.open(address);        //�Զ��ķ�ʽ���ļ�
    if (!in) {               //�ж��Ƿ�ɹ���
		pos.X =73;
		if (pos.Y < 25)  pos.Y = 24;
		else pos.Y = 49;
		SetConsoleCursorPosition(hOutput, pos);
        cout << "ERROR!";
    }
	else {
		char ch[2 << 0];
		in >> ch;                //��ȡ�ļ����ַ���������������ȡ��ʽ���ɸ�����Ҫʹ��
		in.close();              //�ر��ļ�
		cout << ch << endl;
	}
    return 0;
}
/***************************************************************************
  �������ƣ�ShutDown
  ��    �ܣ��˳�����
***************************************************************************/
int Editor::ShutDown() { 
    GetConsoleScreenBufferInfo(hOutput, &csbi);
    pos.X = 0;
    pos.Y = csbi.dwMaximumWindowSize.Y;
    SetConsoleCursorPosition(hOutput, pos);
    Sleep(100);
    exit(0);
}