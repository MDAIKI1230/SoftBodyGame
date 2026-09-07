#pragma once

/// @def SingletonPublicFunction(type,funcName,declaration,arguments)
/// @brief 外部に公開する関数の宣言を省略する用
/// @brief 使用例：
/// @brief /// <summary>
/// @brief /// 3Dモデルロード
/// @brief /// </summary>
/// @brief /// <param name="_path">フルパス</param>
///	@brief static bool LoadModel(std::filesystem::path _path)
///	@brief SingletonPublicFunction(bool, LoadModel, (std::filesystem::path _path), (_path));
/// 
/// @param Type 戻り値
/// @param FuncName 宣言した関数の名前
/// @param declaration 宣言した関数の引数
/// @param arguments 引数を渡すときの並び
/// 
/// @endcode
/// 
/// @warning 実装に当たる関数は、funcName+Implになるので別で実装を書く
/// @warning publicで終わるので注意
#define SingletonPublicFunction(Type,FuncName,declaration,arguments)\
		{\
			return Instance().FuncName##Impl arguments;\
		}\
	private:\
		Type FuncName##Impl declaration; \
	public:\

#define SingletonPublicTemplateFunction(Type,Template,templateArg,FuncName,declaration,arguments)\
		{\
			return Instance().FuncName##Impl<templateArg> arguments;\
		}\
	private:\
		Template\
		Type FuncName##Impl declaration; \
	public:\
