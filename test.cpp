/*
    ��ϣ��ʵ���ı���������
    made by Michael Yeung/����23331161
    �������ڽ�ֹ���ڽ�����Ṳ�����ҵĸ��˲��ͣ�www.notegardens.cn
    2024/12/8
    ��ϣ��д��input1.h��input2.h��
    ����GBK����򿪴��ļ�
*/
#define NOMINMAX
#include "algorithm"
#include "cctype"
#include "fstream"
#include "input1.h"
#include "input2.h"
#include "iostream"
#include "limits"
#include "string"
#include "vector"
#include "windows.h" // ����GBK����

using namespace std;

void search_cn()
{
    ifstream file1("chinese.txt");
    if (!file1)
    {
        cout << "�޷����ļ�!" << endl;
        return;
    }
    vector<string> text;
    string line;
    while (getline(file1, line))
    {
        text.push_back(line);
    }
    file1.close();

    string combined_text;
    for (const auto &line : text)
    {
        combined_text += line;
    }

    vector<string> sentences = splitchinese(combined_text);
    cout << "һ����" << sentences.size() << "������" << endl;

    ChineseHashTable table;

    for (size_t i = 0; i < sentences.size(); i++)
    {
        vector<string> words = extractword1(sentences[i]);
        size_t wordsize = min<size_t>(4, words.size());
        for (size_t len = 1; len <= wordsize; len++)
        {
            for (size_t start = 0; start <= words.size() - len; start++)
            {
                string s;
                for (size_t k = 0; k < len; k++)
                {
                    s += words[start + k];
                }
                table.insert(s, i);
            }
        }
    }

    int cnt = 1;
    while (true)
    {
        cout << "�����������ؼ��ʣ�" << endl;
        string word;
        getline(cin, word);
        cout << "��ѯ " << cnt << "��" << word << endl;
        vector<int> res = table.search(word);
        if (res.empty())
        {
            cout << "û���ҵ����� \"" << word << "\" �ľ��ӡ�" << endl;
        }
        else
        {
            cout << "�ҵ� " << res.size() << " ������ \"" << word << "\" �ľ��ӣ�" << endl;
            cout << "��� " << cnt << "��" << endl;
            for (int idx : res)
            {
                cout << sentences[idx] << endl;
            }
        }
        cnt++;
    }
}

void search_en()
{
    ifstream file2("text.txt");
    if (!file2)
    {
        cout << "Can't open the file!" << endl;
        return;
    }
    vector<string> text;
    string line;
    while (getline(file2, line))
    {
        text.push_back(line);
    }
    file2.close();
    string combined_text;
    for (const auto &line : text)
    {
        combined_text += line + "\n";
    }
    vector<string> sentences = split(combined_text);
    cout << "Total sentences: " << sentences.size() << endl;

    EnglishHashTable table;

    for (size_t i = 0; i < sentences.size(); i++)
    {
        vector<string> words = extractword2(sentences[i]);
        for (const auto &word : words)
        {
            table.insert(word, i);
        }
    }
    int cnt = 1;
    while (true)
    {
        cout << "Please enter search keyboard:" << endl;
        string word;
        getline(cin, word);
        transform(word.begin(), word.end(), word.begin(), ::tolower);
        cout << "Query " << cnt << "��" << word << endl;
        vector<int> res = table.search(word);
        if (res.empty())
        {
            cout << "There are no sentences that contain \"" << word << "\"." << endl;
        }
        else
        {
            cout << "There are " << res.size() << " sentences that contain \"" << word << "\":" << endl;
            cout << "Output" << cnt++ << ":" << endl;
            for (int idx : res)
            {
                cout << sentences[idx] << "." << endl;
            }
        }
    }
}

int main()
{
    SetConsoleOutputCP(936);
    SetConsoleCP(936);

    cout << "��ӭ�����ı���������ϵͳ(Welcome to the Text Search Engine System):" << endl;
    cout << "��ѡ������(Please choose the language):" << endl;
    cout << "1.����" << endl;
    cout << "2.English" << endl;
    int x;
    cin >> x;

    /*
    ��һ�еĴ������Ҫ��������debug�˺ܾ�
    */
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ������뻺�����еĻ��з�

    if (x == 1)
    {
        search_cn();
    }
    else if (x == 2)
    {
        search_en();
    }
    else
    {
        cout << "��������ѡ����Ӵ��������һ��" << endl;
    }
    return 0;
}