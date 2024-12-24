/*
    哈希表实现文本搜索引擎
    made by Michael Yeung/杨振23331161
    本代码在截止日期结束后会共享至我的个人博客：www.notegardens.cn
    2024/12/8
    哈希表写在input1.h和input2.h中
    请用GBK编码打开此文件
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
#include "windows.h" // 处理GBK编码

using namespace std;

void search_cn()
{
    ifstream file1("chinese.txt");
    if (!file1)
    {
        cout << "无法打开文件!" << endl;
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
    cout << "一共有" << sentences.size() << "个句子" << endl;

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
        cout << "请输入搜索关键词：" << endl;
        string word;
        getline(cin, word);
        cout << "查询 " << cnt << "：" << word << endl;
        vector<int> res = table.search(word);
        if (res.empty())
        {
            cout << "没有找到包含 \"" << word << "\" 的句子。" << endl;
        }
        else
        {
            cout << "找到 " << res.size() << " 个包含 \"" << word << "\" 的句子：" << endl;
            cout << "输出 " << cnt << "：" << endl;
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
        cout << "Query " << cnt << "：" << word << endl;
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

    cout << "欢迎进入文本搜索引擎系统(Welcome to the Text Search Engine System):" << endl;
    cout << "请选择语言(Please choose the language):" << endl;
    cout << "1.中文" << endl;
    cout << "2.English" << endl;
    int x;
    cin >> x;

    /*
    下一行的代码很重要，在这里debug了很久
    */
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // 清除输入缓冲区中的换行符

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
        cout << "哈哈，你选错了哟，请再来一次" << endl;
    }
    return 0;
}