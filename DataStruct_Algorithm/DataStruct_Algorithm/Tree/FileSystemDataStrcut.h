#pragma once
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// 파일 시스템의 노드를 나타내는 구조체 정의
typedef struct nArrNode {
    string name;  // 파일 또는 디렉토리의 이름
    bool isDir;  // 이 노드가 디렉토리인지 여부 (true: 디렉토리, false: 파일)
    vector<nArrNode*> children;  // 자식 노드들을 가리키는 포인터들의 벡터 (파일 또는 서브디렉토리)
} nArrNode;

// 파일 시스템을 나타내는 구조체 정의
struct FileSystem
{
    using node = nArrNode;  // nArrNode 구조체를 node라는 이름으로 별칭 정의
    using nodePtr = node*;  // nArrNode 포인터 타입을 nodePtr이라는 이름으로 별칭 정의

private:
    nodePtr root;  // 파일 시스템의 루트 디렉토리를 가리키는 포인터
    nodePtr cwd;  // 현재 작업 디렉토리를 가리키는 포인터

public:
    FileSystem();  // 파일 시스템 생성자, 루트 디렉토리 초기화
    nodePtr find(const string& path);  // 주어진 경로에 해당하는 노드를 찾는 함수
    bool add(const string& path, bool isDir);  // 주어진 경로에 파일 또는 디렉토리를 추가하는 함수
    bool ChangeDir(const string path);  // 현재 작업 디렉토리를 변경하는 함수
    void ShowPath(const string& path);  // 주어진 경로의 파일 및 디렉토리들을 출력하는 함수

private:
    nodePtr FindImpl(const nodePtr directory, const string& path);  // 재귀적으로 노드를 찾는 내부 구현 함수
    bool addImpl(nodePtr directory, const string& path, bool isDir);  // 재귀적으로 노드를 추가하는 내부 구현 함수
};
