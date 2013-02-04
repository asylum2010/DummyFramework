//=============================================================================================================
#ifndef _CHELPER_H_
#define _CHELPER_H_

#include "../template/defines.h"

#include <string>
#include <vector>

#if _DUMMY_DX == 9
#	include <d3d9.h>
#	include <d3dx9.h>
#elif _DUMMY_DX == 10
#	include <d3d10.h>
#	include <d3dx10.h>
#endif

namespace DummyFramework
{
	/**
	 * \brief Helper class
	 */
	class CHelper
	{
	public:
		static const float Pi;
		static const float TwoPi;
		static const float PiHalf;
		static const D3DXMATRIX Identity;

		static bool GaussEliminate(float out[], unsigned char params, float** A, float b[]);

		static float InvSqrt(float x);
		static float Gaussian(float x, float mean, float stddev);
		static float SineLength(float a, float b, unsigned short precision = 20);

		static void GaussianOffsets(int sample, float dim, float** buffer);
		static void GaussianWeights(int sample, float multiplier, float mean, float stddev, float** buffer);
		static void Reflect(D3DXVECTOR3& out, const D3DXVECTOR3& v, const D3DXPLANE& p);
		static void Reflect(D3DXVECTOR3& out, const D3DXVECTOR3& v, const D3DXVECTOR3& a);
		static void Split(std::vector<std::string>& out, char separator, const std::string& str, bool keepempty);
		static void Split(std::vector<std::string>& out, const std::string& separators, const std::string& str, bool keepempty);

		static std::string& GetFile(std::string &out, const std::string& str);
		static std::string& GetName(std::string &out, const std::string& str);
		static std::string& GetPath(std::string &out, const std::string& str);
		static std::string& GetExtension(std::string &out, const std::string& str);
		static std::string& Crop(std::string& out, const std::string& str, char ch);
		static std::string& Replace(std::string& out, const std::string& what, const std::string& with, const std::string& str);
		static std::string& Replace(std::string& out, char what, char with, const std::string& str);

		static std::string FormatToString(D3DFORMAT format);

		template <typename T>
		static std::string DiscreteToString(T value);

		template <typename T>
		static bool Clamp2(T& value, T min, T max);

		template <typename T>
		inline static T Clamp(T value, T min, T max) {
			return (value < min ? min : (value > max ? max : value));
		}

		inline static tagPOINT MakePoint(int x, int y) {
			tagPOINT p = { x, y };
			return p;
		}

		inline static float fsign(float f) {
			return (f == 0 ? 0.0f : (f < 0 ? -1.0f : 1.0f));
		}

		inline static void MirrorX(float out[4], float texcoords[4]) {
			out[0] = texcoords[2];
			out[1] = texcoords[1];
			out[2] = texcoords[0];
			out[3] = texcoords[3];
		}

		inline static void MirrorY(float out[4], float texcoords[4]) {
			out[0] = texcoords[0];
			out[1] = texcoords[3];
			out[2] = texcoords[2];
			out[3] = texcoords[1];
		}

		// requires uv flip
		inline static void MirrorYX(float out[4], float texcoords[4]) {
			out[0] = texcoords[0];
			out[1] = texcoords[1];
			out[2] = texcoords[3];
			out[3] = texcoords[2];
		}

		// requires uv flip
		inline static void MirrorXY(float out[4], float texcoords[4]) {
			out[0] = texcoords[2];
			out[1] = texcoords[3];
			out[2] = texcoords[1];
			out[3] = texcoords[0];
		}
	};

	template <typename T>
	static std::string CHelper::DiscreteToString(T value)
	{
		std::string str("           ");
		bool neg = (value < 0);
		unsigned char index = 10;

		if( neg )
			value = 0 - value;

		do
		{
			str[index] = (char)((value % 10) + 0x30);
			value /= 10;
			--index;
		}
		while( value != 0 );

		if( neg )
			str[index] = '-';

		return Crop(str, str, ' ');
	}

	template <typename T>
	static bool CHelper::Clamp2(T& value, T min, T max) {
		bool ret = false;

		if( value < min )
		{
			value = min;
			ret = true;
		}

		if( value > max )
		{
			value = max;
			ret = true;
		}

		return ret;
	}
}

#endif
//=============================================================================================================
