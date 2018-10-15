#pragma once


// json helpers /////////////////////////////////////////////////////////////////


inline void jsonSaveHotkey(unsigned hk, Json::Value& v)
{
	v.append(hk & 0xff);	hk >>= 8;
	v.append(hk & 0xff);	hk >>= 8;
	v.append(hk & 0xff);	hk >>= 8;
	v.append(hk & 0xff);	hk >>= 8;
}

inline void jsonLoadHotkey(unsigned& hk, const Json::Value& v)
{
	if (v.isArray())
	{
		hk = 0;
		hk |= v[3].asUInt(); hk <<= 8;
		hk |= v[2].asUInt(); hk <<= 8;
		hk |= v[1].asUInt(); hk <<= 8;
		hk |= v[0].asUInt();
	}
	//TODO for compatibility, remove in october 2018
/*
	else
		if (v.isString())
			hk = STRtoDWORD(v.asCString());
*/
}

inline void jsonLoadColor(DWORD& clr, const Json::Value& v)
{
	if (v.isArray())
	{
		clr = 0;
		clr |= v[3].asUInt(); clr <<= 8;
		clr |= v[2].asUInt(); clr <<= 8;
		clr |= v[1].asUInt(); clr <<= 8;
		clr |= v[0].asUInt();
	}
	//TODO for compatibility, remove in october 2018
/*
	else
		if (v.isString())
			clr = STRtoDWORD(v.asCString());
*/
}

inline void jsonSaveColor(unsigned clr, Json::Value& v)
{
	v.append(clr & 0xff);	clr >>= 8;
	v.append(clr & 0xff);	clr >>= 8;
	v.append(clr & 0xff);	clr >>= 8;
	v.append(clr & 0xff);	clr >>= 8;
}

inline void jsonSave(const Gdiplus::SolidBrush* brush, Json::Value& v)
{
	if (brush)
	{
		Gdiplus::Color clr = 0;
		brush->GetColor(&clr);
		jsonSaveColor(clr.GetValue(), v);
	}
}

inline void jsonSave(const char* var, Json::Value& v)
{
	v = var;
}

inline void jsonSave(int var, Json::Value& v)
{
	v = var;
}

inline void jsonSave(unsigned var, Json::Value& v)
{
	v = var;
}

inline void jsonSave(bool var, Json::Value& v)
{
	v = var;
}

inline void jsonSave(float var, Json::Value& v)
{
	v = var;
}

inline void jsonLoad(int& var, const Json::Value& value, bool acceptNull = true)
{
	if (value.isNull() && !acceptNull)
		return;
	if (value.isConvertibleTo(Json::intValue))	var = value.asInt();
	else
		if (value.isString())	var = atoi(value.asCString());
}

inline void jsonLoad(unsigned& var, const Json::Value& value, bool acceptNull = true)
{
	if (value.isNull() && !acceptNull)
		return;
	if (value.isConvertibleTo(Json::uintValue))	var = value.asUInt();
	else
		if (value.isString())	var = atoi(value.asCString());
}

inline void jsonLoad(float& var, const Json::Value& value)
{
	if (value.isConvertibleTo(Json::realValue))	var = value.asFloat();
	else
		if (value.isString())	var = float(atof(value.asCString()));
}


inline void jsonLoad(bool& var, const Json::Value& value, bool acceptNull = true)
{
	if (value.isNull() && !acceptNull)
		return;
	if (value.isConvertibleTo(Json::booleanValue))
		var = value.asBool();
	else
		if (value.isString())
			var = atoi(value.asCString()) != 0;
}

GAMEHOOK_API void jsonLoad(char* var, size_t varsize, const Json::Value& value, bool acceptNull = true);

inline Gdiplus::SolidBrush* jsonLoadBrush(const Json::Value& value)
{
	string sv;
	if (value.isNull())
		return nullptr;
	DWORD clr;
	jsonLoadColor(clr, value);
	return new Gdiplus::SolidBrush(Gdiplus::Color(clr));
}

inline void jsonLoadBrush(shared_ptr<Gdiplus::SolidBrush>& br, const Json::Value& value)
{
	auto pBrush = jsonLoadBrush(value);
	if (pBrush)
		br.reset(pBrush);
}

inline void jsonSaveBrush(shared_ptr<Gdiplus::SolidBrush>& br, Json::Value& value)
{
	if (br.get())
	{
		Gdiplus::Color clr;
		br->GetColor(&clr);
		jsonSaveColor(clr.GetValue(), value);
	}
}
