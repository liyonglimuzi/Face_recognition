#include "stdafx.h"
#include "BrowseDir.h"
#include "direct.h"  
//#include "string.h"  
#include "io.h"  
#include "stdio.h"   
#include <vector>  
#include <iostream> 
#define _STDC_WANT_LIB_EXET_ 1
#include<string.h>

using namespace std;

//using namespace cv;

CBrowseDir::CBrowseDir()

{
	_getcwd(m_szInitDir, _MAX_PATH);
	int len = strlen(m_szInitDir);
	if (m_szInitDir[len - 1] != '\\')
	{
		strcat_s(m_szInitDir, "\\");

	}


}
bool CBrowseDir::SetInitDir(const char *dir)
{
	if (_fullpath(m_szInitDir, dir, _MAX_PATH) == NULL)
	{
		return false;
	}
	if (_chdir(m_szInitDir) != 0)
		return false;
	int len = strlen(m_szInitDir);
	if (m_szInitDir[len - 1] != '\\')
		strcat_s(m_szInitDir, "\\");
	return true;


}

vector<string>CBrowseDir::BeginBrowseFilenames(const char *filespes)
{
	ProcessDir(m_szInitDir, NULL);
	return GetDirFilenames(m_szInitDir, filespes);

}
bool CBrowseDir::BeginBrowse(const char* filespes)
{
	ProcessDir(m_szInitDir, NULL);
	return BrowseDir(m_szInitDir, filespes);
}
bool CBrowseDir::BrowseDir(const char *dir, const char *filespes)
{
	_chdir(dir);
	long hFile;
	_finddata_t fileinfo;
	if (hFile = _findfirst(filespes, &fileinfo) != -1)
	{
		do
		{
			if (!(fileinfo.attrib &_A_SUBDIR))//目录不为空
			{
				char filename[_MAX_PATH];
				strcpy_s(filename, dir);
				strcat_s(filename, fileinfo.name);
				//	cout << filename << endl;
				if (!ProcessFile(filename))
					return false;
			}

		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);

	}
	_chdir(dir);
	if (hFile = _findfirst("*.*", &fileinfo) != -1)
	{
		do
		{
			if ((fileinfo.attrib &_A_SUBDIR))
			{
				if ((strcmp(fileinfo.name, ".") != 0) && (strcmp(fileinfo.name, "..") != 0))
				{
					char subdir[_MAX_PATH];
					strcpy_s(subdir, dir);
					strcat_s(subdir, fileinfo.name);
					strcat_s(subdir, "\\");
					ProcessDir(subdir, dir);
					if (!BrowseDir(subdir, filespes))
						return false;
				}
			}

		} while ((_findnext(hFile, &fileinfo)) == 0);
		_findclose(hFile);
	}
	return true;
}
vector<string> CBrowseDir::GetDirFilenames(const char *dir, const char *filespec)
{
	_chdir(dir);
	vector<string>filename_vec;
	filename_vec.clear();
	int i = 0;
	long hFile;
	_finddata_t fileinfo;
	if ((hFile = _findfirst(filespec, &fileinfo)) != -1)
	{

		do
		{
			if (!(fileinfo.attrib & _A_SUBDIR))
			{
				char filename[_MAX_PATH];
				strcpy_s(filename, dir);
				//int st = 0;  
				//while (dir[st++]!='\0');  
				strcat_s(filename, sizeof(filename), fileinfo.name);


				filename_vec.push_back(filename);

				/*	cout << filename_vec.at(i++) << endl;
				for (int i = 0; i < filename_vec.size(); i++) /// 第一种调用方法
				{
				cout << filename_vec.at(i) << endl;
				}*/
			}

		} while (_findnext(hFile, &fileinfo) == 0);


		_findclose(hFile);
	}

	_chdir(dir);
	if ((hFile = _findfirst("*.*", &fileinfo)) != -1)
	{
		do
		{
			if ((fileinfo.attrib & _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp
					(fileinfo.name, "..") != 0)
				{
					char subdir[_MAX_PATH];
					strcpy_s(subdir, dir);
					strcat_s(subdir, fileinfo.name);
					strcat_s(subdir, "\\");
					ProcessDir(subdir, dir);
					return GetDirFilenames(subdir, filespec);
				}
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}

	//cout << filename_vec[3] << endl;
	return filename_vec;
}

bool CBrowseDir::ProcessFile(const char *filename)
{
	return true;
}

void CBrowseDir::ProcessDir(const char
	*currentdir, const char *parentdir)
{
}
CBrowseDir::~CBrowseDir()
{


}
