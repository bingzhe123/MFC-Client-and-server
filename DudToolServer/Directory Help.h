#ifndef DIRECTORY_H_
#define DIRECTORY_H_
#include <stdio.h>
#include <string>
#include <string.h>
#include <iostream>
#ifdef _WIN32
#include <direct.h>		//for mkdir rmdir
#include <io.h>			//for access
#elif __linux__
#include <unistd.h>		//for mkdir rmdir
#include <sys/stat.h>	//for access
#include <dirent.h>		//for DIR remove
#endif

#ifdef _WIN32
#define ACCESS _access
#define MKDIR(a) _mkdir((a))
#define RMDIR(a) _rmdir((a))
#elif __linux__
#define ACCESS access
#define MKDIR(a) mkdir((a),0755)
#define RMDIR(a) rmdir((a))
#endif


static bool MkDir(const std::string& strPath)
{
    int i = 0;
    auto nDirLen = strPath.length();
    if (nDirLen <= 0)
        return false;
    char* pDirTemp = new char[nDirLen + 4];
    strPath.copy(pDirTemp, nDirLen + 1, 0);// +1 to copy '\0'
    pDirTemp[nDirLen] = '\0';
    //��ĩβ��'/'
    if (pDirTemp[nDirLen - 1] != '\\' && pDirTemp[nDirLen - 1] != '/')
    {
        pDirTemp[nDirLen] = '/';
        pDirTemp[nDirLen + 1] = '\0';
        nDirLen++;
    }
    // ����Ŀ¼
    for (i = 0; i < nDirLen; i++)
    {
        if (pDirTemp[i] == '\\' || pDirTemp[i] == '/')
        {
            pDirTemp[i] = '\0';//�ضϺ������Ŀ¼���𼶲鿴Ŀ¼�Ƿ���ڣ����������򴴽�
            //���������,����
            int statu;
            statu = ACCESS(pDirTemp, 0);
            if (statu != 0)//���ܴ���ͬ���ļ�����û�д���
            {
                statu = MKDIR(pDirTemp);
                if (statu != 0)//�����ϼ������ļ��ж���ͬ���ļ����´���ʧ��
                {
                    return false;
                }
            }
            //֧��linux,������\����/
            pDirTemp[i] = '/';
        }
    }
    delete[] pDirTemp;
    return true;
}

static bool RmDir(const std::string& path)
{
    std::string strPath = path;
#ifdef _WIN32
    struct _finddata_t fb;   //������ͬ�����ļ��Ĵ洢�ṹ��
    //������������·��
    if (strPath.at(strPath.length() - 1) != '\\' || strPath.at(strPath.length() - 1) != '/')
        strPath.append("\\");
    std::string findPath = strPath + "*";
    intptr_t handle;//��long���ͻᱨ��
    handle = _findfirst(findPath.c_str(), &fb);
    //�ҵ���һ��ƥ����ļ�
    if (handle != -1L)
    {
        std::string pathTemp;
        do//ѭ���ҵ����ļ� 
        {
            //ϵͳ�и�ϵͳ�ļ�����Ϊ��..���͡�.��,������������  
            if (strcmp(fb.name, "..") != 0 && strcmp(fb.name, ".") != 0)//��ϵͳ�����ļ��Ĵ�����
            {
                //��������·��
                pathTemp.clear();
                pathTemp = strPath + std::string(fb.name);
                //����ֵΪ16����˵�����ļ��У�����  
                if (fb.attrib == _A_SUBDIR)//_A_SUBDIR=16
                {
                    RmDir(pathTemp.c_str());
                }
                //���ļ��е��ļ���ֱ��ɾ�������ļ�����ֵ�����û����ϸ���飬���ܻ������������  
                else
                {
                    remove(pathTemp.c_str());
                }
            }
        } while (0 == _findnext(handle, &fb));//�жϷ�ǰ���ʧȥ��һ�������Ľ��
        //�ر��ļ��У�ֻ�йر��˲���ɾ����������������˺ܾã���׼c���õ���closedir  
        //������ܣ�һ�����Handle�ĺ���ִ�к󣬶�Ҫ���йرյĶ�����  
        _findclose(handle);
    }
    //�Ƴ��ļ���  
    return RMDIR(strPath.c_str()) == 0 ? true : false;

#elif __linux__
    if (strPath.at(strPath.length() - 1) != '\\' || strPath.at(strPath.length() - 1) != '/')
        strPath.append("/");
    DIR* d = opendir(strPath.c_str());//�����Ŀ¼
    if (d != NULL)
    {
        struct dirent* dt = NULL;
        while (dt = readdir(d))//�����ȡĿ¼�е��ļ���dt
        {
            //ϵͳ�и�ϵͳ�ļ�����Ϊ��..���͡�.��,������������
            if (strcmp(dt->d_name, "..") != 0 && strcmp(dt->d_name, ".") != 0)//�ж��Ƿ�Ϊϵͳ�����ļ�
            {
                struct stat st;//�ļ�����Ϣ
                std::string fileName;//�ļ����е��ļ���
                fileName = strPath + std::string(dt->d_name);
                stat(fileName.c_str(), &st);
                if (S_ISDIR(st.st_mode))
                {
                    RmDir(fileName);
                }
                else
                {
                    remove(fileName.c_str());
                }
            }
        }
        closedir(d);
    }
    return rmdir(strPath.c_str()) == 0 ? true : false;
#endif

}


#endif