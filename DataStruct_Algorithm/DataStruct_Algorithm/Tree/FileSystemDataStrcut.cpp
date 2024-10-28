#include "FileSystemDataStrcut.h"

// FileSystem Ŭ���� ������: ��Ʈ ���丮�� �ʱ�ȭ�ϰ� ���� �۾� ���丮�� ��Ʈ�� ����
FileSystem::FileSystem()
{
    root = new node{ "/", true, {} };  // ��Ʈ ���丮 ���� (name: "/", isDir: true, �ڽ� ��� ����)
    cwd = root;  // ���� �۾� ���丮�� ��Ʈ�� ����
}

// ��ο� �ش��ϴ� ��带 ã�� �Լ�
FileSystem::nodePtr FileSystem::find(const string& path)
{
    if (path[0] == '/') {  // ���� ����� ���
        return FindImpl(root, path.substr(1));  // ��Ʈ���� �����Ͽ� Ž��
    }
    else {  // ��� ����� ���
        return FindImpl(cwd, path);  // ���� �۾� ���丮���� �����Ͽ� Ž��
    }
}

// �־��� ���丮���� ��θ� ��������� Ž���Ͽ� ��带 ã�� �Լ�
FileSystem::nodePtr FileSystem::FindImpl(const nodePtr directory, const string& path)
{
    if (path.empty()) return directory;  // ��ΰ� ��� ������ ���� ���丮�� ��ȯ

    auto sep = path.find('/');  // ���� ��� ������ ��ġ ã��
    string currentPath = sep == string::npos ? path : path.substr(0, sep);  // ���� Ž���� ��� �κ�
    string restPath = sep == string::npos ? "" : path.substr(sep + 1);  // ���� ��� �κ�

    // ���� ���丮�� �ڽ� ��忡�� �ش� �̸��� ���� ��带 ã��
    auto found = find_if(directory->children.begin(), directory->children.end(),
        [&](const nodePtr child) {
            return child->name == currentPath;
        });

    if (found != directory->children.end()) {
        return FindImpl(*found, restPath);  // ã�� ���, ������ ��ο� ���� ��������� Ž��
    }

    return NULL;  // ã�� ���� ��� NULL ��ȯ
}

// ��ο� �� ���� �Ǵ� ���丮�� �߰��ϴ� �Լ�
bool FileSystem::add(const string& path, bool isDir)
{
    if (path[0] == '/') {  // ���� ����� ���
        return addImpl(root, path.substr(1), isDir);  // ��Ʈ���� �����Ͽ� �߰�
    }
    else {  // ��� ����� ���
        return addImpl(cwd, path, isDir);  // ���� �۾� ���丮���� �����Ͽ� �߰�
    }
}

// �־��� ���丮�� ��ο� ���� �� ���� �Ǵ� ���丮�� �߰��ϴ� �Լ� (����� ����)
bool FileSystem::addImpl(nodePtr directory, const string& path, bool isDir)
{
    if (not directory->isDir) {  // ���� ���丮�� ������ ���
        cout << directory->name << "��(��) �����Դϴ�." << endl;
        return false;
    }

    auto sep = path.find('/');  // ���� ��� ������ ��ġ ã��

    if (sep == string::npos) {  // ������ ��� �κ��� ���
        // ���� ���丮�� �ڽ� ��� �� ������ �̸��� ���� ��尡 �ִ��� Ȯ��
        auto found = find_if(directory->children.begin(), directory->children.end(),
            [&](const nodePtr child) {
                return child->name == path;
            });

        if (found != directory->children.end()) {  // ������ �̸��� ����/���丮�� �̹� �ִ� ���
            cout << directory->name << "�� �̹� " << path << " �̸��� ����/���͸��� �ֽ��ϴ�." << endl;
            return false;
        }

        // ���ο� ���� �Ǵ� ���丮�� ���� ���丮�� �߰�
        directory->children.push_back(new node{ path, isDir, {} });
        return true;
    }

    // ���� ��� �κ� ����
    string nextDir = path.substr(0, sep);
    // ���� ���丮�� �ڽ� ��� �� ���� ��ο� �ش��ϴ� ���丮 ã��
    auto found = find_if(directory->children.begin(), directory->children.end(),
        [&](const nodePtr child) {
            return child->name == nextDir && child->isDir;
        });

    if (found != directory->children.end()) {  // ���丮�� ã�� ���, ������ ��ο� ���� ��������� �߰�
        return addImpl(*found, path.substr(sep + 1), isDir);
    }

    cout << directory->name << "�� " << nextDir << " �̸��� ���͸��� �����ϴ�." << endl;
    return false;
}

// ���� �۾� ���丮�� �����ϴ� �Լ�
bool FileSystem::ChangeDir(const string path)
{
    auto found = find(path);  // �־��� ��ο� �ش��ϴ� ��带 ã��
    if (found && found->isDir) {  // ���丮�� ���
        cwd = found;  // ���� �۾� ���丮�� ����
        cout << "���� ���͸��� " << cwd->name << "�� �̵��մϴ�." << endl;
        return true;
    }

    cout << path << " ��θ� ã�� �� �����ϴ�." << endl;
    return false;
}

// �־��� ��ο� �ִ� ���� �� ���丮���� ����ϴ� �Լ�
void FileSystem::ShowPath(const string& path)
{
    auto found = find(path);  // �־��� ��ο� �ش��ϴ� ��带 ã��
    if (not found) {  // ��ΰ� �������� �ʴ� ���
        cout << path << " ��ΰ� �������� �ʽ��ϴ�." << endl;
        return;
    }

    if (found->isDir) {  // ���丮�� ���, �ڽ� ������ ���
        for (auto child : found->children) {
            cout << (child->isDir ? "d " : "- ") << child->name << endl;
        }
    }
    else {  // ������ ���, ���� �̸��� ���
        cout << "- " << found->name << endl;
    }
}
