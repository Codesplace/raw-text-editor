#include "head.h"

int Editor::BM_Search(string str)
{
    string ReText=GetText(loc.X, loc.Y);
    SetConsoleCursorPosition(hOutput, pos);
    int Re = str_bm(&ReText[0], ReText.size(), &str[0], str.size());
    if (Re > 0)
    {
        int e = Re + loc.X;
        int y = e / 80;
        int x = e % 80;
        pos.X = x;
        pos.Y = loc.Y + y;
        SetConsoleCursorPosition(hOutput, pos);
    }
    return 0;
}
string  Editor::GetText(int j, int i) {
    string str;
    str += text[i].substr(j);
    i++;
    for (; i < 50; i++) {
        str += text[i];
    }
    return str;
}
int Editor::generateBadChar(char* b, int m, int* badchar)//(ģʽ���ַ�b��ģʽ������m��ģʽ���Ĺ�ϣ��)
{
    int i, ascii;
    for (i = 0; i < 256; ++i)
    {
        badchar[i] = -1;//��ϣ���ʼ��Ϊ-1
    }
    for (i = 0; i < m; ++i)
    {
        ascii = int(b[i]);  //�����ַ���ASCIIֵ
        badchar[ascii] = i;//�ظ��ַ������ǣ���¼���������ֵĸ��ַ���λ��
    }
    return 0;
}
int Editor::generateGS(char* b, int m, int* suffix, bool* prefix)//Ԥ����ģʽ�������suffix��prefix
{
    int i, j, k;
    for (i = 0; i < m; ++i)//���������ʼ��
    {
        suffix[i] = -1;
        prefix[i] = false;
    }
    for (i = 0; i < m - 1; ++i)//b[0,i]
    {
        j = i;
        k = 0;//������׺�Ӵ�����(ģʽ��β��ȡk���������ֱ�Ƚ�)
        while (j >= 0 && b[j] == b[m - 1 - k])//��b[0,m-1]�󹫹���׺�Ӵ�
        {
            --j;
            ++k;
            suffix[k] = j + 1;
            //��ͬ��׺�Ӵ�����Ϊkʱ�����Ӵ���b[0,i]�е���ʼ�±�
            // (����ж����ͬ���ȵ��Ӵ�������ֵ���ǣ���ϴ��)
        }
        if (j == -1)//���ҵ�ģʽ����ͷ����
            prefix[k] = true;//���������׺�Ӵ�Ҳ��ģʽ����ǰ׺�Ӵ�
    }
    return 0;
}
int Editor::moveByGS(int j, int m, int* suffix, bool* prefix)//�����j�ǻ��ַ���Ӧ��ģʽ���е��ַ��±�
{
    int k = m - 1 - j;//�ú�׺����
    if (suffix[k] != -1)//case1���ҵ����ú�׺һ����ģʽ�Ӵ�������Ļ�����Ŀ�����Ǹ����Ӵ���ʼ�±꣩��
        return j - suffix[k] + 1;
    for (int r = j + 2; r < m; ++r)//case2
    {
        if (prefix[m - r] == true)//m-r�Ǻú�׺���Ӵ��ĳ��ȣ��������ú�׺���Ӵ���ģʽ����ǰ׺�Ӵ�
            return r;//������û���ҵ���ͬ�ĺú�׺�£��ƶ�rλ������ǰ׺���ú�׺
    }
    return m;//case3,��û��ƥ��ģ��ƶ�mλ��ģʽ�����ȣ�
}
int Editor::str_bm(char* a, int n, char* b, int m)//a��ʾ��������n; b��ʾģʽ��,��m
{
    int* badchar = new int[256];//��¼ģʽ����ÿ���ַ������ֵ�λ��
    generateBadChar(b, m, badchar);     //�������ַ���ϣ��
    int* suffix = new int[m];
    bool* prefix = new bool[m];
    generateGS(b, m, suffix, prefix);   //Ԥ����ģʽ�������suffix��prefix
    int i = 0, j, moveLen1, moveLen2;//j��ʾ������ģʽ��ƥ��ĵ�һ���ַ�
    while (i < n - m + 1)
    {
        for (j = m - 1; j >= 0; --j)  //ģʽ���Ӻ���ǰƥ��
        {
            if (a[i + j] != b[j])
                break;  //���ַ���Ӧģʽ���е��±���j
        }
        if (j < 0)   //ƥ��ɹ�
        {
            delete[] badchar;
            delete[] suffix;
            delete[] prefix;
            return i;   //����������ģʽ����һ��ƥ����ַ���λ��
        }
        //�����ͬ�ڽ�ģʽ�����󻬶� j-badchar[int(a[i+j])] λ
        moveLen1 = j - badchar[int(a[i + j])];//���ջ��ַ������ƶ�����
        moveLen2 = 0;
        if (j < m - 1)//����кú�׺�Ļ�
        {
            moveLen2 = moveByGS(j, m, suffix, prefix);//���պú�׺�����ƶ�����
        }
        i = i + max(moveLen1, moveLen2);//ȡ����ƶ�
    }
    delete[] badchar;
    delete[] suffix;
    delete[] prefix;
    return -1;
}