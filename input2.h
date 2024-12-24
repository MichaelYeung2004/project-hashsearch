/*
    支持英文
*/
#ifndef INPUT2_H
#define INPUT2_H

#include <algorithm>
#include <cctype>
#include <fcntl.h>
#include <fstream>
#include <io.h>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct Node
{
    string word;
    vector<int> sentences;
    Node *next;
    Node(const string &str, int idx) : word(str), sentences{idx}, next(nullptr) {}
};

// 这里我们采用拉链法来实现哈希表
class EnglishHashTable
{
public:
    vector<Node *> data; // 数组空间
    int cnt, size;       // cnt表示当前元素数量，size表示哈希表大小

    // BKDR哈希函数
    int bkdrhash(const string &s) const
    {
        int seed = 131;
        long long h = 0;
        for (char c : s)
        {
            h = h * seed + c;
        }
        return h & 0x7fffffff;
    }
    void swap(EnglishHashTable &other)
    {
        std::swap(data, other.data);
        std::swap(cnt, other.cnt);
        std::swap(size, other.size);
    }
    EnglishHashTable(int n = 99999) : cnt(0), size(n)
    {
        data.resize(size, nullptr);
    }
    ~EnglishHashTable()
    {
        for (int i = 0; i < size; ++i)
        {
            Node *p = data[i];
            while (p)
            {
                Node *q = p->next;
                delete p;
                p = q;
            }
            data[i] = nullptr;
        }
        cnt = 0;
    }

    void insert(const string &word, int idx)
    {
        if (cnt >= size * 2)
        {
            EnglishHashTable newTable(size * 2);
            for (int i = 0; i < size; ++i)
            {
                Node *p = data[i];
                while (p)
                {
                    // 遍历所有句子
                    for (int sentence : p->sentences)
                    {
                        newTable.insert(p->word, sentence);
                    }
                    p = p->next;
                }
            }
            swap(newTable);
        }
        int hcode = bkdrhash(word);
        int ind = hcode % size;
        Node *p = data[ind];
        while (p)
        {
            if (p->word == word)
            {
                // 排除同一个句子因为包含两个相同单词而被重复计数的情况
                if (find(p->sentences.begin(), p->sentences.end(), idx) == p->sentences.end())
                {
                    p->sentences.push_back(idx);
                }
                return;
            }
            p = p->next;
        }
        Node *newNode = new Node(word, idx);
        newNode->next = data[ind];
        data[ind] = newNode;
        cnt++;
    }

    // 搜索单词
    vector<int> search(const string &word) const
    {
        int hcode = bkdrhash(word);
        int ind = hcode % size;
        Node *p = data[ind];
        while (p)
        {
            if (p->word == word)
            {
                return p->sentences;
            }
            p = p->next;
        }
        return {};
    }
};

// 分句函数
vector<string> split(const string &text)
{
    vector<string> sentences;
    string sentence;
    for (char c : text)
    {
        // 跳过非ASCII字符（包括中文）
        if ((unsigned char)c > 127)
        {
            continue;
        }

        if (c == '.' || c == '!' || c == '?')
        {
            if (!sentence.empty())
            {
                // 去除句子前后的空格
                int start = sentence.find_first_not_of(" \n\r\t");
                int end = sentence.find_last_not_of(" \n\r\t");
                sentences.push_back(sentence.substr(start, end - start + 1));
                sentence.clear();
            }
        }
        else
        {
            sentence += c;
        }
    }
    // 处理最后一个没有标点的句子
    if (!sentence.empty())
    {
        int start = sentence.find_first_not_of(" \n\r\t");
        int end = sentence.find_last_not_of(" \n\r\t");
        sentences.push_back(sentence.substr(start, end - start + 1));
    }
    return sentences;
}

// 分词函数
vector<string> extractword2(const string &sentence)
{
    vector<string> words;
    string word;
    for (char c : sentence)
    {
        if (isspace(c))
        {
            if (!word.empty())
            {
                words.push_back(word);
                word.clear();
            }
        }
        else
        {
            if (isalpha(c))
            {
                word += tolower(c); // 转换为小写以统一处理
            }
        }
    }
    // 处理最后一个单词
    if (!word.empty())
    {
        words.push_back(word);
    }
    return words;
}

#endif