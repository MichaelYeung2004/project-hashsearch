#ifndef CN_INPUT1_H
#define CN_INPUT1_H

#include "algorithm"
#include "cctype"
#include "fstream"
#include "iostream"
#include "limits"
#include "string"
#include "vector"
#include "windows.h" // 用于设置编码
#include <fcntl.h>
#include <io.h>
using namespace std;

struct ChineseNode
{
    string word;
    vector<int> sentences;
    ChineseNode *next;
    ChineseNode(const string &str, int idx) : word(str), sentences{idx}, next(nullptr) {}
};

class ChineseHashTable
{
public:
    vector<ChineseNode *> data;
    int cnt, size;

    // bkdr哈希函数
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
    void swap(ChineseHashTable &other)
    {
        std::swap(data, other.data);
        std::swap(cnt, other.cnt);
        std::swap(size, other.size);
    }
    ChineseHashTable(int n = 10007) : cnt(0), size(n)
    {
        data.resize(size, nullptr);
    }

    ~ChineseHashTable()
    {
        for (int i = 0; i < size; ++i)
        {
            ChineseNode *p = data[i];
            while (p)
            {
                ChineseNode *q = p->next;
                delete p;
                p = q;
            }
            data[i] = nullptr;
        }
        cnt = 0;
    }

    void insert(const string &word, int sentence)
    {
        if (cnt >= size * 2)
        {
            ChineseHashTable newTable(size * 2);
            for (int i = 0; i < size; ++i)
            {
                ChineseNode *p = data[i];
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
        ChineseNode *p = data[ind];
        while (p)
        {
            if (p->word == word)
            {
                if (find(p->sentences.begin(), p->sentences.end(), sentence) == p->sentences.end())
                {
                    p->sentences.push_back(sentence);
                }
                return;
            }
            p = p->next;
        }
        ChineseNode *newNode = new ChineseNode(word, sentence);
        newNode->next = data[ind];
        data[ind] = newNode;
        cnt++;
    }

    vector<int> search(const string &word) const
    {
        int hcode = bkdrhash(word);
        int ind = hcode % size;
        ChineseNode *p = data[ind];
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
vector<string> splitchinese(const string &text)
{
    vector<string> sentences;
    string sentence;
    for (size_t i = 0; i < text.size();)
    {
        // 处理GBK字符，每个字符占用2个字节
        int clen = 1;
        if (i + 1 < text.size() && ((unsigned char)text[i] > 0x80))
        {
            // GBK的高字节大于0x80时，表示是一个双字节字符
            clen = 2;
        }
        string tempStr = text.substr(i, clen);
        i += clen;
        sentence += tempStr;
        if (tempStr == "。" || tempStr == "." ||
            tempStr == "！" || tempStr == "!" ||
            tempStr == "？" || tempStr == "?" ||
            tempStr == "；" || tempStr == ";")
        {

            if (!sentence.empty())
            {
                sentences.push_back(sentence);
                sentence.clear();
            }
        }
    }

    // 处理最后一个没有标点的句子
    if (!sentence.empty())
    {
        sentences.push_back(sentence);
    }
    return sentences;
}

// 分词函数
vector<string> extractword1(const string &sentence)
{
    vector<string> words;
    string word;
    for (size_t i = 0; i < sentence.length();)
    {
        // 处理GBK字符，每个字符占用2个字节
        int clen = 1;
        if (i + 1 < sentence.size() && ((unsigned char)sentence[i] > 0x80))
        {
            // GBK的高字节大于0x80时，表示是一个双字节字符
            clen = 2;
        }
        word = sentence.substr(i, clen);
        i += clen;
        if (!word.empty())
        {
            words.push_back(word);
        }
    }
    return words;
}

#endif
