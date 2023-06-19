#include <filesystem>
#include <vector>
#include <algorithm>
#include <string>
#include <regex>
namespace fs = std::filesystem;
using namespace std;

uintmax_t get_directory_size(const fs::path& dir_path)
{
    uintmax_t size = 0;
    for (auto& p : fs::recursive_directory_iterator(dir_path)) {
        if (p.is_regular_file()) {
            size += fs::file_size(p.path());
        }
    }
    return size;
}

std::wstring removeSubStr(std::wstring source) {

    std::wstring::size_type pos;
    pos = source.find(L"(");
    if (pos != std::wstring::npos) {
        source = source.substr(0, pos - 1);
        return source;
    }

    pos = source.find(L"#");
    if (pos != std::wstring::npos) {
        source = source.substr(0, pos - 1);
        return source;
    }

    return source;
}

bool findLegacy(const wstring& source) {
    wregex pattern(L"Legacy");  // The pattern to search for
    wsmatch match_results;

    return regex_search(source, match_results, pattern);
}

void SortFolderByName(fs::path path_to_read, vector<fs::directory_entry>& entries);
fs::path Rename(fs::path old_dir_path, wstring _newName);

int folderCount = 0;
int prevCount = 0;
int main()
{   
    fs::path basePath = "C:/Users/skdye/OneDrive/바탕 화면/Animator2";

    fs::path path_to_read(basePath);
    vector<fs::directory_entry> entries;
    
    // 폴더 읽어오기
    for (auto& entry : fs::directory_iterator(path_to_read))
        entries.push_back(entry);   

    // 폴더 정렬
    SortFolderByName(path_to_read, entries);

    // 여기서 entries는 이름 순으로 정렬된 상태입니다.

    fs::path sour = "";
    fs::path dest = "";
    wstring wstrSour = sour.filename().wstring();
    wstring wstrDest = dest.filename().wstring();
    uintmax_t sourSize;
    uintmax_t destSize;
    for (auto& entry : fs::directory_iterator(path_to_read))
    {
        dest = entry;
        
        // Delete Legacy Folder
        if (findLegacy(dest.filename().wstring()))
        {
            fs::remove_all(dest);
        }

        wstrSour = removeSubStr(sour.filename().wstring());
        wstrDest = removeSubStr(dest.filename().wstring());

        if (wstrSour != wstrDest)
        {
            sour = dest;
        }
        else
        {
            sourSize = get_directory_size(sour);
            destSize = get_directory_size(dest);
            if (sourSize == destSize)
            {
                fs::remove_all(sour);
                sour = dest;
            }
            else if (sourSize > destSize)
            {
                fs::remove_all(dest);
            }
            else
            {
                fs::remove_all(sour);
                sour = dest;
            }
        }
    }

    for (auto& entry : fs::directory_iterator(path_to_read))
    {
        fs::path foloder = entry;
        Rename(entry, removeSubStr(foloder.filename().wstring()));
    }
}

void SortFolderByName(fs::path path_to_read, vector<fs::directory_entry>& entries)
{
    std::sort(entries.begin(), entries.end(),
        [](const fs::directory_entry& a, const fs::directory_entry& b)
        {
            return a.path().filename().wstring() < b.path().filename().wstring();
        });
}

fs::path Rename(fs::path old_dir_path,  wstring _newName)
{
    wstring newName = _newName.c_str();
    fs::path new_dir_path = old_dir_path.parent_path().wstring() + L"/" + newName;
    if (!fs::exists(new_dir_path))
        fs::rename(old_dir_path, new_dir_path);

    if (folderCount != prevCount)
        return new_dir_path;

    return new_dir_path;
}