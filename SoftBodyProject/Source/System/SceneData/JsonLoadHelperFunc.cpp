#include "JsonLoadHelperFunc.h"

#include "TransformComponentData.h"
#include "BoxColliderComponentData.h"
#include "SphereColliderComponentData.h"


std::unique_ptr<SceneComponentData> JsonLoadHelperFunc::CreateTransformData(const rapidjson::Value& _value)
{
    // 出力データ
	std::unique_ptr<TransformComponentData> data{ std::make_unique<TransformComponentData>() };
	
    // データを詰めていく。
    GetVector3(_value["position"], data->position);
    GetVector3(_value["rotationDeg"], data->position);
    GetVector3(_value["scale"], data->position);

    return data;
}

std::unique_ptr<SceneComponentData> JsonLoadHelperFunc::CreateBoxColliderData(const rapidjson::Value& _value)
{
    // 出力データ
    std::unique_ptr<BoxColliderComponentData> data{ std::make_unique<BoxColliderComponentData>() };

    // データを詰めていく。
    GetVector3(_value["size"], data->size);

    return data;
}

std::unique_ptr<SceneComponentData> JsonLoadHelperFunc::CreateSphereColliderData(const rapidjson::Value& _value)
{
    // 出力データ
    std::unique_ptr<SphereColliderComponentData> data{ std::make_unique<SphereColliderComponentData>() };

    // データを詰めていく。
    data->radius = _value["radius"].GetFloat();

    return data;
}

bool JsonLoadHelperFunc::GetVector3(const rapidjson::Value& _value, Vector3& _output)
{
    // 引数にオブジェクトが来ているかチェック
    if (!_value.IsObject())
    {
        return false;
    }

    // Vector3の要素があるかチェック
    if (!_value.HasMember("x") || !_value["x"].IsNumber())
    {
        return false;
    }
    if (!_value.HasMember("y") || !_value["y"].IsNumber())
    {
        return false;
    }
    if (!_value.HasMember("z") || !_value["z"].IsNumber())
    {
        return false;
    }

    // 値渡して終了
    _output = Vector3{_value["x"].GetFloat(),_value["y"].GetFloat(), _value["z"].GetFloat()};

    return true;
}
