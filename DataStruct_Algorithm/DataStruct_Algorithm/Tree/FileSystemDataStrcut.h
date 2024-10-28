#pragma once
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// ���� �ý����� ��带 ��Ÿ���� ����ü ����
typedef struct nArrNode {
    string name;  // ���� �Ǵ� ���丮�� �̸�
    bool isDir;  // �� ��尡 ���丮���� ���� (true: ���丮, false: ����)
    vector<nArrNode*> children;  // �ڽ� ������ ����Ű�� �����͵��� ���� (���� �Ǵ� ������丮)
} nArrNode;

// ���� �ý����� ��Ÿ���� ����ü ����
struct FileSystem
{
    using node = nArrNode;  // nArrNode ����ü�� node��� �̸����� ��Ī ����
    using nodePtr = node*;  // nArrNode ������ Ÿ���� nodePtr�̶�� �̸����� ��Ī ����

private:
    nodePtr root;  // ���� �ý����� ��Ʈ ���丮�� ����Ű�� ������
    nodePtr cwd;  // ���� �۾� ���丮�� ����Ű�� ������

public:
    FileSystem();  // ���� �ý��� ������, ��Ʈ ���丮 �ʱ�ȭ
    nodePtr find(const string& path);  // �־��� ��ο� �ش��ϴ� ��带 ã�� �Լ�
    bool add(const string& path, bool isDir);  // �־��� ��ο� ���� �Ǵ� ���丮�� �߰��ϴ� �Լ�
    bool ChangeDir(const string path);  // ���� �۾� ���丮�� �����ϴ� �Լ�
    void ShowPath(const string& path);  // �־��� ����� ���� �� ���丮���� ����ϴ� �Լ�

private:
    nodePtr FindImpl(const nodePtr directory, const string& path);  // ��������� ��带 ã�� ���� ���� �Լ�
    bool addImpl(nodePtr directory, const string& path, bool isDir);  // ��������� ��带 �߰��ϴ� ���� ���� �Լ�
};
