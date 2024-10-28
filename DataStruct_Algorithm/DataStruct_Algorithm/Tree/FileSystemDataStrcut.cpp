#include "FileSystemDataStrcut.h"

// FileSystem 클래스 생성자: 루트 디렉토리를 초기화하고 현재 작업 디렉토리를 루트로 설정
FileSystem::FileSystem()
{
    root = new node{ "/", true, {} };  // 루트 디렉토리 생성 (name: "/", isDir: true, 자식 노드 없음)
    cwd = root;  // 현재 작업 디렉토리를 루트로 설정
}

// 경로에 해당하는 노드를 찾는 함수
FileSystem::nodePtr FileSystem::find(const string& path)
{
    if (path[0] == '/') {  // 절대 경로인 경우
        return FindImpl(root, path.substr(1));  // 루트에서 시작하여 탐색
    }
    else {  // 상대 경로인 경우
        return FindImpl(cwd, path);  // 현재 작업 디렉토리에서 시작하여 탐색
    }
}

// 주어진 디렉토리에서 경로를 재귀적으로 탐색하여 노드를 찾는 함수
FileSystem::nodePtr FileSystem::FindImpl(const nodePtr directory, const string& path)
{
    if (path.empty()) return directory;  // 경로가 비어 있으면 현재 디렉토리를 반환

    auto sep = path.find('/');  // 다음 경로 구분자 위치 찾기
    string currentPath = sep == string::npos ? path : path.substr(0, sep);  // 현재 탐색할 경로 부분
    string restPath = sep == string::npos ? "" : path.substr(sep + 1);  // 남은 경로 부분

    // 현재 디렉토리의 자식 노드에서 해당 이름을 가진 노드를 찾음
    auto found = find_if(directory->children.begin(), directory->children.end(),
        [&](const nodePtr child) {
            return child->name == currentPath;
        });

    if (found != directory->children.end()) {
        return FindImpl(*found, restPath);  // 찾은 경우, 나머지 경로에 대해 재귀적으로 탐색
    }

    return NULL;  // 찾지 못한 경우 NULL 반환
}

// 경로에 새 파일 또는 디렉토리를 추가하는 함수
bool FileSystem::add(const string& path, bool isDir)
{
    if (path[0] == '/') {  // 절대 경로인 경우
        return addImpl(root, path.substr(1), isDir);  // 루트에서 시작하여 추가
    }
    else {  // 상대 경로인 경우
        return addImpl(cwd, path, isDir);  // 현재 작업 디렉토리에서 시작하여 추가
    }
}

// 주어진 디렉토리에 경로에 따라 새 파일 또는 디렉토리를 추가하는 함수 (재귀적 구현)
bool FileSystem::addImpl(nodePtr directory, const string& path, bool isDir)
{
    if (not directory->isDir) {  // 현재 디렉토리가 파일인 경우
        cout << directory->name << "은(는) 파일입니다." << endl;
        return false;
    }

    auto sep = path.find('/');  // 다음 경로 구분자 위치 찾기

    if (sep == string::npos) {  // 마지막 경로 부분인 경우
        // 현재 디렉토리의 자식 노드 중 동일한 이름을 가진 노드가 있는지 확인
        auto found = find_if(directory->children.begin(), directory->children.end(),
            [&](const nodePtr child) {
                return child->name == path;
            });

        if (found != directory->children.end()) {  // 동일한 이름의 파일/디렉토리가 이미 있는 경우
            cout << directory->name << "에 이미 " << path << " 이름의 파일/디렉터리가 있습니다." << endl;
            return false;
        }

        // 새로운 파일 또는 디렉토리를 현재 디렉토리에 추가
        directory->children.push_back(new node{ path, isDir, {} });
        return true;
    }

    // 다음 경로 부분 추출
    string nextDir = path.substr(0, sep);
    // 현재 디렉토리의 자식 노드 중 다음 경로에 해당하는 디렉토리 찾기
    auto found = find_if(directory->children.begin(), directory->children.end(),
        [&](const nodePtr child) {
            return child->name == nextDir && child->isDir;
        });

    if (found != directory->children.end()) {  // 디렉토리를 찾은 경우, 나머지 경로에 대해 재귀적으로 추가
        return addImpl(*found, path.substr(sep + 1), isDir);
    }

    cout << directory->name << "에 " << nextDir << " 이름의 디렉터리가 없습니다." << endl;
    return false;
}

// 현재 작업 디렉토리를 변경하는 함수
bool FileSystem::ChangeDir(const string path)
{
    auto found = find(path);  // 주어진 경로에 해당하는 노드를 찾음
    if (found && found->isDir) {  // 디렉토리인 경우
        cwd = found;  // 현재 작업 디렉토리를 변경
        cout << "현재 디렉터리를 " << cwd->name << "로 이동합니다." << endl;
        return true;
    }

    cout << path << " 경로를 찾을 수 없습니다." << endl;
    return false;
}

// 주어진 경로에 있는 파일 및 디렉토리들을 출력하는 함수
void FileSystem::ShowPath(const string& path)
{
    auto found = find(path);  // 주어진 경로에 해당하는 노드를 찾음
    if (not found) {  // 경로가 존재하지 않는 경우
        cout << path << " 경로가 존재하지 않습니다." << endl;
        return;
    }

    if (found->isDir) {  // 디렉토리인 경우, 자식 노드들을 출력
        for (auto child : found->children) {
            cout << (child->isDir ? "d " : "- ") << child->name << endl;
        }
    }
    else {  // 파일인 경우, 파일 이름만 출력
        cout << "- " << found->name << endl;
    }
}
