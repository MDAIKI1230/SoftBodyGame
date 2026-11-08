#include "JsonLoadHelperFunc.h"

#include "TransformComponentData.h"
#include "BoxColliderComponentData.h"
#include "SphereColliderComponentData.h"
#include "RendererComponentData.h"


std::unique_ptr<SceneComponentData> JsonLoadHelperFunc::CreateTransformData(const rapidjson::Value& _value)
{
    // 出力データ
	std::unique_ptr<TransformComponentData> data{ std::make_unique<TransformComponentData>() };
	
    // データを詰めていく。
    GetVector3(_value["Position"], data->position);
    GetVector3(_value["RotationDeg"], data->ratationDeg);
    GetVector3(_value["Scale"], data->scale);

    return data;
}

std::unique_ptr<SceneComponentData> JsonLoadHelperFunc::CreateBoxColliderData(const rapidjson::Value& _value)
{
    // 出力データ
    std::unique_ptr<BoxColliderComponentData> data{ std::make_unique<BoxColliderComponentData>() };

    // データを詰めていく。
    GetVector3(_value["Size"], data->size);

	if (_value.HasMember("OffsetPosition"))
	{
		GetVector3(_value["OffsetPosition"], data->offsetPosition);
	}
	if (_value.HasMember("OffsetRotationDeg"))
	{
		GetVector3(_value["OffsetRotationDeg"], data->offsetRotationDeg);
	}

    return data;
}

std::unique_ptr<SceneComponentData> JsonLoadHelperFunc::CreateSphereColliderData(const rapidjson::Value& _value)
{
    // 出力データ
    std::unique_ptr<SphereColliderComponentData> data{ std::make_unique<SphereColliderComponentData>() };

    // データを詰めていく。
    data->radius = _value["Radius"].GetFloat();

	if (_value.HasMember("OffsetPosition"))
	{
		GetVector3(_value["OffsetPosition"], data->offsetPosition);
	}
	if (_value.HasMember("OffsetRotationDeg"))
	{
		GetVector3(_value["OffsetRotationDeg"], data->offsetRotationDeg);
	}

    return data;
}

std::unique_ptr<SceneComponentData> JsonLoadHelperFunc::CreateRendererData(const rapidjson::Value& _value)
{
	std::unique_ptr<RendererComponentData> data{ std::make_unique<RendererComponentData>() };

	if (_value.HasMember("ModelPath") && _value["ModelPath"].IsString())
	{
		data->modelPath = _value["ModelPath"].GetString();
	}

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
    if (!_value.HasMember("X") || !_value["X"].IsNumber())
    {
        return false;
    }
    if (!_value.HasMember("Y") || !_value["Y"].IsNumber())
    {
        return false;
    }
    if (!_value.HasMember("Z") || !_value["Z"].IsNumber())
    {
        return false;
    }

    // 値渡して終了
    _output = Vector3{_value["X"].GetFloat(),_value["Y"].GetFloat(), _value["Z"].GetFloat()};

    return true;
}
