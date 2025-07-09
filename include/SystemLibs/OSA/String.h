#ifndef __IMOUOS_OSAPI_STRING_H__
#define __IMOUOS_OSAPI_STRING_H__
#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
#include "OSA/IntTypes.h"
#include "OSA/ImousTypes.h"

typedef imouos_handle imouos_str;

#define IMOUOS_STR_NPOS ((Uint32)-1)

/**
* 安全字符串拷贝
* 当长度不足时，不会拷贝，并将dest[0]置为NULL
* @param dst    目标buffer
* @param size   buffer总长度
* @param src    源字符串
* @param n      拷贝长度(不包括NULL终止符)，-1时自动截断并追加NULL
* @return       0：成功；<0：失败；>0：字符串被截断
*/
Int32 IMOUOS_API imouos_strncpy(char* dest, Sizet size, const char* src, Sizet n);

/**
* 安全计算字符串长度
* 计算字符串长度，最多访问maxlen字节
* 参数为空时，返回0
* @warning  strnlen_safe并非strlen的替代函数。
*           其被用来计算在已知大小缓冲区中的不被信任的外部数据长度
*           strnlen_safe逐字节访问字符串，因此比strlen更慢
* @param s      目标字符串
* @param maxlen 最大计算长度
* @return       字符串长度
*/
Sizet IMOUOS_API imouos_strnlen(const char* s, Sizet maxlen);

/**
* 字符串安全快速拼接
* @param dst    目标区域
* @param size   目标区域长度
* @param src    源字符串
* @param n      源字符串长度
* @return       0：成功；<0：失败
*/
Int32 IMOUOS_API imouos_strncat(char* dst, Sizet size, const char* src, Sizet n);


/// 创建字符串副本，传入NULL创建空字符串
/// \param [in]  str        源字符串
/// \return                 字符串副本
imouos_str IMOUOS_API imouos_strCreate(const imouos_str str);

/// 创建数据副本
/// \param [in]  data       数据
/// \param [in]  len        数据长度
/// \return                 字符串
imouos_str IMOUOS_API imouos_strCreateCpoy(const Uint8* data, Uint32 len);

/// 创建字符串副本
/// \param [in]  str        源字符串
/// \param [in]  pos        起始位置，超出时创建失败
/// \param [in]  len        最大拷贝长度
/// \return                 字符串副本
imouos_str IMOUOS_API imouos_strCreateCopyLen(const imouos_str str, Uint32 pos, Uint32 len);

/// 创建字符串副本
/// \param [in]  str        源字符串
/// \return                 字符串副本
imouos_str IMOUOS_API imouos_strCreateStr(const char* str);

/// 创建字符串副本
/// \param [in]  str        源字符串
/// \param [in]  len        最大拷贝长度
/// \return                 字符串副本
imouos_str IMOUOS_API imouos_strCreateStrLen(const char* str, Uint32 len);

/// 创建字符串副本
/// \param [in]  n          字符数量
/// \param [in]  c          字符
/// \return                 字符串
imouos_str IMOUOS_API imouos_strCreateNChar(Uint32 n, char c);

/// 销毁字符串
/// \return                 0：成功；<0：失败
Int32 IMOUOS_API imouos_strDestroy(imouos_str str);

/// 追加数据
/// \param [in]  str        字符串
/// \param [in]  s          目标字符串
/// \return                 0：成功；<0：失败
Int32 IMOUOS_API imouos_strAppend(imouos_str str, const imouos_str s);

/// 追加二进制数据
/// \param [in]  str        字符串
/// \param [in]  data       数据
/// \param [in]  len        数据长度
/// \return                 0：成功；<0：失败
Int32 IMOUOS_API imouos_strAppendBin(imouos_str str, const Uint8* data, Uint32 len);

/// 追加数据
/// \param [in]  str        字符串
/// \param [in]  s          目标字符串
/// \param [in]  subpos     起始位置
/// \param [in]  sublen     追加长度
/// \return                 0：成功；<0：失败
Int32 IMOUOS_API imouos_strAppendSubposStrLen(imouos_str str, const imouos_str s, Uint32 subpos, Uint32 sublen);

/// 追加数据
/// \param [in]  str        字符串
/// \param [in]  s          目标字符串
/// \return                 0：成功；<0：失败
Int32 IMOUOS_API imouos_strAppendStr(imouos_str str, const char* s);

/// 追加数据
/// \param [in]  str        字符串
/// \param [in]  s          源字符串
/// \param [in]  n          最大拷贝长度
/// \return                 0：成功；<0：失败
Int32 IMOUOS_API imouos_strAppendStrLen(imouos_str str, const char* s, Uint32 n);

/// 追加数据
/// \param [in]  str        字符串
/// \param [in]  n          字符数量
/// \param [in]  c          字符
/// \return                 0：成功；<0：失败
Int32 IMOUOS_API imouos_strAppendNChar(imouos_str str, Uint32 n, char c);

/// 字符串赋值
/// \param [in]  str        字符串
/// \param [in]  s          源字符串
/// \return                 0：成功；<0：失败
Int32 IMOUOS_API imouos_strAssign(imouos_str str, const imouos_str s);

/// 赋值二进制数据
/// \param [in]  str        字符串
/// \param [in]  data       二进制数据
/// \param [in]  len        数据长度
/// \return                 0：成功；<0：失败
Int32 IMOUOS_API imouos_strAssignBin(imouos_str str, const Uint8* data, Uint32 len);

/// 字符串赋值
/// \param [in]  str        字符串
/// \param [in]  s          源字符串
/// \param [in]  subpos     源字符串起始位置
/// \param [in]  sublen     元字符串拷贝长度
/// \return                 0：成功；<0：失败
Int32 IMOUOS_API imouos_strAssignSubposStrLen(imouos_str str, const imouos_str s, Uint32 subpos, Uint32 sublen);

/// 字符串赋值
/// \param [in]  str        字符串
/// \param [in]  s          源字符串
/// \return                 0：成功；<0：失败
Int32 IMOUOS_API imouos_strAssignStr(imouos_str str, const char* s);

/// 字符串赋值
/// \param [in]  str        字符串
/// \param [in]  s          源字符串
/// \param [in]  n          源字符串拷贝长度
/// \return                 0：成功；<0：失败
Int32 IMOUOS_API imouos_strAssignStrLen(imouos_str str, const char* s, Uint32 n);

/// 字符串赋值
/// \param [in]  str        字符串
/// \param [in]  len        赋值长度
/// \param [in]  c          赋值数据
/// \return                 0：成功；<0：失败
Int32 IMOUOS_API imouos_strAssignNChar(imouos_str str, Uint32 len, char c);

/// 将数字赋值给字符串（十进制格式）
/// \param [in]  str        字符串
/// \param [in]  _dec       数字
/// \return                 0：成功；<0：失败
Int32 IMOUOS_API imouos_strAssignNumber(imouos_str str, Uint32 _dec);

/// 字符串插入
/// \param [in]  str        字符串
/// \param [in]  pos        插入位置
/// \param [in]  s          字符串
/// \return                 0：成功；<0：失败
Int32 IMOUOS_API imouos_strInsert(imouos_str str, Uint32 pos, const imouos_str s);

/// 二进制数据插入
/// \param [in]  str        字符串
/// \param [in]  pos        插入位置
/// \param [in]  data       二进制数据
/// \param [in]  n          数据长度
/// \return                 0：成功；<0：失败
Int32 IMOUOS_API imouos_strInsertBin(imouos_str str, Uint32 pos, const Uint8* data, Uint32 n);

/// 字符串插入
/// \param [in]  str        字符串
/// \param [in]  pos        插入位置
/// \param [in]  s          字符串
/// \param [in]  subpos     起始位置
/// \param [in]  sublen     拷贝长度
/// \return                 0：成功；<0：失败
Int32 IMOUOS_API imouos_strInsertPosStrLen(imouos_str str, Uint32 pos, const imouos_str s, Uint32 subpos, Uint32 sublen);

/// 字符串插入
/// \param [in]  str        字符串
/// \param [in]  pos        插入位置
/// \param [in]  s          字符串
/// \return                 0：成功；<0：失败
Int32 IMOUOS_API imouos_strInsertStr(imouos_str str, Uint32 pos, const char* s);

/// 字符串插入
/// \param [in]  str        字符串
/// \param [in]  pos        插入位置
/// \param [in]  s          字符串
/// \param [in]  n          拷贝长度
/// \return                 0：成功；<0：失败
Int32 IMOUOS_API imouos_strInsertStrLen(imouos_str str, Uint32 pos, const char* s, Uint32 n);

/// 字符插入
/// \param [in]  str        字符串
/// \param [in]  pos        插入位置
/// \param [in]  n          插入数量
/// \param [in]  c          插入字符
/// \return                 0：成功；<0：失败
Int32 IMOUOS_API imouos_strInsertNChar(imouos_str str, Uint32 pos, Uint32 n, char c);

/// 字符串替换
/// \param [in]  str        字符串
/// \param [in]  pos        替换位置
/// \param [in]  len        替换长度
/// \param [in]  s          字符串
/// \return                 0：成功；<0：失败
Int32 IMOUOS_API imouos_strReplace(imouos_str str, Uint32 pos, Uint32 len, const imouos_str s);

/// 二进制替换
/// \param [in]  str        字符串
/// \param [in]  pos        替换位置
/// \param [in]  len        替换长度
/// \param [in]  data       二进制数据
/// \param [in]  n          数据长度
/// \return                 0：成功；<0：失败
Int32 IMOUOS_API imouos_strReplaceBin(imouos_str str, Uint32 pos, Uint32 len, const Uint8* data, Uint32 n);

/// 字符串替换
/// \param [in]  str        字符串
/// \param [in]  pos        替换位置
/// \param [in]  len        替换长度
/// \param [in]  s          字符串
/// \param [in]  subpos     起始替换位置
/// \param [in]  sublen     长度
/// \return                 0：成功；<0：失败
Int32 IMOUOS_API imouos_strReplacePosStrLen(imouos_str str, Uint32 pos, Uint32 len, const imouos_str s, Uint32 subpos, Uint32 sublen);

/// 字符串替换
/// \param [in]  str        字符串
/// \param [in]  pos        替换位置
/// \param [in]  len        替换长度
/// \param [in]  s          字符串
/// \return                 0：成功；<0：失败
Int32 IMOUOS_API imouos_strReplaceStr(imouos_str str, Uint32 pos, Uint32 len, const char* s);

/// 字符串替换
/// \param [in]  str        字符串
/// \param [in]  pos        替换位置
/// \param [in]  len        替换长度
/// \param [in]  s          字符串
/// \param [in]  n          字符串长度
/// \return                 0：成功；<0：失败
Int32 IMOUOS_API imouos_strReplaceStrLen(imouos_str str, Uint32 pos, Uint32 len, const char* s, Uint32 n);

/// 字符替换
/// \param [in]  str        字符串
/// \param [in]  pos        起始替换位置
/// \param [in]  len        替换长度
/// \param [in]  n          字符数量
/// \param [in]  c          替换字符
/// \return                 0：成功；<0：失败
Int32 IMOUOS_API imouos_strReplaceNChar(imouos_str str, Uint32 pos, Uint32 len, Uint32 n, char c);

/// 查找字符串
/// \param [in] str     字符串
/// \param [in] pos     起始查找位置
/// \param [in] key     目标字符串
/// \return             >=0：匹配位置；<0：失败
Int32 IMOUOS_API imouos_strFind(const imouos_str str, Uint32 pos, const imouos_str key);

/// 查找二进制数据
/// \param [in] str     字符串
/// \param [in] pos     起始查找位置
/// \param [in] len     查找长度
/// \param [in] key     目标数据
/// \param [in] keyLen  数据长度
/// \return             >=0：匹配位置；<0：失败
Int32 IMOUOS_API imouos_strFindBin(const imouos_str str, Uint32 pos, Uint32 len, const Uint8* key, Uint32 keyLen);

/// 查找字符串
/// \param [in] str     字符串
/// \param [in] pos     起始查找位置
/// \param [in] key     目标字符串
/// \return             >=0：匹配位置；<0：失败
Int32 IMOUOS_API imouos_strFindStr(const imouos_str str, Uint32 pos, const char* key);

/// 查找字符串
/// \param [in] str     字符串
/// \param [in] pos     起始查找位置
/// \param [in] key     目标字符串
/// \param [in] n       字符串长度
/// \return             >=0：匹配位置；<0：失败
Int32 IMOUOS_API imouos_strFindStrLen(const imouos_str str, Uint32 pos, const char* key, Uint32 n);

/// 查找字符
/// \param [in] str     字符串
/// \param [in] pos     起始查找位置
/// \param [in] c       字符
/// \return             >=0：匹配位置；<0：失败
Int32 IMOUOS_API imouos_strFindChar(const imouos_str str, Uint32 pos, char c);

/// 字符串对比
/// \param [in] str     源字符串
/// \param [in] s       目标字符串
/// \return             0：相等；!0：不等
Int32 IMOUOS_API imouos_strCompare(const imouos_str str, const imouos_str s);

/// 字符串对比
/// \param [in] str     源字符串
/// \param [in] pos     起始匹配位置
/// \param [in] len     最大匹配长度
/// \param [in] data    目标数据
/// \param [in] datalen 目标数据长度
/// \return             0：相等；!0：不等
Int32 IMOUOS_API imouos_strCompareBin(const imouos_str str, Uint32 pos, Uint32 len, const Uint8* data, Uint32 datalen);

/// 字符串对比
/// \param [in] str     源字符串
/// \param [in] pos     起始匹配位置
/// \param [in] len     匹配长度
/// \param [in] s       目标字符串
/// \return             0：相等；!0：不等
Int32 IMOUOS_API imouos_strCompareLen(const imouos_str str, Int32 pos, Int32 len, const imouos_str s);

/// 字符串对比
/// \param [in] str     源字符串
/// \param [in] pos     起始匹配位置
/// \param [in] len     最大匹配长度
/// \param [in] s       目标字符串
/// \param [in] subpos  目标字符串起始匹配位置
/// \param [in] sublen  目标字符串匹配长度
/// \return             0：相等；!0：不等
Int32 IMOUOS_API imouos_strComparePosStrLen(const imouos_str str, Uint32 pos, Uint32 len, const imouos_str s, Uint32 subpos, Uint32 sublen);

/// 字符串对比
/// \param [in] str     源字符串
/// \param [in] s       目标字符串
/// \return             0：相等；!0：不等
Int32 IMOUOS_API imouos_strCompareStr(const imouos_str str, const char* s);

/// 字符串对比
/// \param [in] str     源字符串
/// \param [in] pos     起始匹配位置
/// \param [in] len     匹配长度
/// \param [in] s       目标字符串
/// \return             0：相等；!0：不等
Int32 IMOUOS_API imouos_strCompareStrLen(const imouos_str str, Uint32 pos, Uint32 len, const char* s);

/// 字符串对比
/// \param [in] str     源字符串
/// \param [in] pos     起始匹配位置
/// \param [in] len     匹配长度
/// \param [in] s       目标字符串
/// \param [in] n       匹配长度
/// \return             0：相等；!0：不等
Int32 IMOUOS_API imouos_strCompareNChar(const imouos_str str, Uint32 pos, Uint32 len, const char* s, Uint32 n);

/// 将字符串转换为Int32类型数据
/// 指定范围必须仅由0~9组成，起始字符允许为'-'
/// \param [in]  str    字符串
/// \param [in]  pos    起始位置
/// \param [in]  len    最大解析长度
/// \param [out] dest   目标数据
/// \return             0：成功；<0：失败
Int32 IMOUOS_API imouos_strToInt32(const imouos_str str, Uint32 pos, Uint32 len, Int32* dest);

/// 将字符串转换为Uint32类型数据
/// 指定范围必须仅由0~9组成
/// \param [in]  str    字符串
/// \param [in]  pos    起始位置
/// \param [in]  len    最大解析长度
/// \param [out] dest   目标数据
/// \return             0：成功；<0：失败
Int32 IMOUOS_API imouos_strToUInt32(const imouos_str str, Uint32 pos, Uint32 len, Uint32* dest);

/// 将字符串转换为int64_t类型数据
/// 指定范围必须仅由0~9组成，起始字符允许为'-'
/// \param [in]  str    字符串
/// \param [in]  pos    起始位置
/// \param [in]  len    最大解析长度
/// \param [out] dest   目标数据
/// \return             0：成功；<0：失败
Int32 IMOUOS_API imouos_strToInt64(const imouos_str str, Uint32 pos, Uint32 len, Int64* dest);

/// 将字符串转换为uint64_t类型数据
/// 指定范围必须仅由0~9组成
/// \param [in]  str    字符串
/// \param [in]  pos    起始位置
/// \param [in]  len    最大解析长度
/// \param [out] dest   目标数据
/// \return             0：成功；<0：失败
Int32 IMOUOS_API imouos_strToUInt64(const imouos_str str, Uint32 pos, Uint32 len, Uint64* dest);

/// 清空字符串
/// \param [in]  str    字符串
/// \return             0：成功；<0：失败
Int32 IMOUOS_API imouos_strClear(imouos_str str);

/// 获取用于存储字符串的区块大小
/// \param [in]  str    字符串
/// \return             区块大小
Int32 IMOUOS_API imouos_strCapacity(const imouos_str str);

/// 获取字符串长度，不包括NULL结束符
/// \param [in]  str    字符串
/// \return             字符串长度
Int32 IMOUOS_API imouos_strSize(const imouos_str str);

/// 获取原始以NULL结尾的字符串。此字符串内部数据不可修改
/// \param [in]  str    字符串
/// \return             C字符串
const char* IMOUOS_API imouos_strData(const imouos_str str);

/// 将str所占用的capacity减少至size大小。
/// 此操作不会影响字符串长度以及内容
/// \param [in]  str    字符串
/// \return             0：成功；<0：失败
Int32 IMOUOS_API imouos_strShrink(imouos_str str);

/// 交换两个字符串的内容
/// \param [in] str     字符串
/// \param [in] s       字符串
/// \return             0：成功；<0：失败
Int32 IMOUOS_API imouos_strSwap(imouos_str str, imouos_str s);

/// 删除字符串中指定数据段
/// \param [in] str     字符串
/// \param [in] pos     删除起始位置
/// \param [in] len     删除长度
/// \return             0：成功；<0：失败
Int32 IMOUOS_API imouos_strErase(imouos_str str, Uint32 pos, Uint32 len);

/// 将字符串拷贝至缓冲区中，拷贝内容不包括NULL结束符
/// \param [in]  str        源字符串
/// \param [in]  pos        字符串起始位置
/// \param [out] buffer     缓冲区
/// \param [in]  len        缓冲区长度
/// \return                 实际拷贝的字节数量；<0：失败
Int32 IMOUOS_API imouos_strCopyLen(imouos_str str, Uint32 pos, char* buffer, Uint32 len);

#ifdef __cplusplus
}
#endif
#endif
