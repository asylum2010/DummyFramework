//=============================================================================================================
#include <dummyframework/base/CHelper.h>
#include <algorithm>
#include <cmath>

namespace DummyFramework
{
    const float CHelper::Pi = 3.141592654f;
    const float CHelper::TwoPi = 6.283185308f;
    const float CHelper::PiHalf = 1.570796327f;
    const D3DXMATRIX CHelper::Identity(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
    //=============================================================================================================
    bool CHelper::GaussEliminate(float out[], unsigned char params, float** A, float b[])
    {
        unsigned char max;
        float gamma;

        for( unsigned char k = 0; k < params - 1; ++k )
        {
            // megkeressük a legnagyobb elemet
            max = k;

            for( unsigned char i = k + 1; i < params; ++i )
            {
                if( abs(A[i][k]) > abs(A[max][k]) )
                    max = i;
            }

            // sorcsere
            if( A[max][k] != 0 )
            {
                std::swap(A[k], A[max]);
                std::swap(b[k], b[max]);
            }
            else
                return false;

            for( unsigned char i = k + 1; i < params; ++i )
            {
                gamma = (A[i][k] / A[k][k]);
                b[i] = b[i] - gamma * b[k];

                for( unsigned char j = k; j < params; ++j )
                    A[i][j] = A[i][j] - gamma * A[k][j];
            }
        }

        // a kapott felsõháromszög mátrixbol kiszámolhatóak a paraméterek
        for( unsigned char i = params - 1; i != 0xff; --i )
        {
            out[i] = b[i];

            for( unsigned char j = i + 1; j < params; ++j )
                out[i] -= A[i][j] * out[j];

            if( A[i][i] == 0 )
                return false;

            out[i] /= A[i][i];
        }

        return true;
    }
    //=============================================================================================================
    float CHelper::InvSqrt(float x)
    {
        float xhalf = 0.5f * x;

        union
        {
            float x;
            long i;
        } u;

        u.x = x;
        u.i = 0x5f375a86 - (u.i >> 1);
        x = u.x * (1.5f - xhalf * u.x * u.x);

        return x;
    }
    //=============================================================================================================
    float CHelper::Gaussian(float x, float mean, float stddev)
    {
        return (InvSqrt(TwoPi * stddev * stddev)) 
                * expf((-((x - mean)*(x - mean))) / (2.0f * stddev * stddev));
    }
    //=============================================================================================================
    float CHelper::SineLength(float a, float b, unsigned short precision)
    {
        float length = 0;
        float step = (b - a) / precision;
        float x = a + step;
        float z = sin(a);
        float y = sin(x) - z;

        for( int i = 0; i < precision; ++i )
        {
            length += sqrt(step * step + y * y);
            z = sin(x);

            x += step;
            y = sin(x) - z;
        }

        return length;
    }
    //=============================================================================================================
    void CHelper::GaussianOffsets(int sample, float dim, float** buffer)
    {
        (*buffer) = new float[sample];

        if( !(*buffer) )
            return;

        int sp2 = sample / 2;

        for( int i = 0; i < sample; ++i )
            (*buffer)[i] = ((float)(i - sp2)) * (1.0f / dim);
    }
    //=============================================================================================================
    void CHelper::GaussianWeights(int sample, float multiplier, float mean, float stddev, float** buffer)
    {
        (*buffer) = new float[sample];

        if( !(*buffer) )
            return;

        float sp2 = (float)(sample / 2);

        for( int i = 0; i < sample; ++i )
        {
            float x = ((float)i - sp2);
            (*buffer)[i] = multiplier * Gaussian(x, mean, stddev);
        }
    }
    //=============================================================================================================
    void CHelper::Reflect(D3DXVECTOR3& out, const D3DXVECTOR3& v, const D3DXPLANE& p)
    {
        float u = 2.0f * (v.x * p.a + v.y * p.b + v.z * p.c - p.d);

        out.x = v.x - p.a * u;
        out.y = v.y - p.b * u;
        out.z = v.z - p.c * u;
    }
    //=============================================================================================================
    void CHelper::Reflect(D3DXVECTOR3& out, const D3DXVECTOR3& v, const D3DXVECTOR3& a)
    {
        out = 2.0f * D3DXVec3Dot(&v, &a) * a - v;
    }
    //=============================================================================================================
    void CHelper::Split(std::vector<std::string>& out, char separator, const std::string& str, bool keepempty)
    {
        out.clear();
        out.reserve(5);

        size_t start = 0;
        size_t pos;
    
        do
        {
            pos = str.find(separator, start);
            
            if( keepempty || pos - start > 0 )
            {
                if( out.capacity() == out.size() )
                    out.reserve(out.size() + 5);

                out.push_back(str.substr(start, pos - start));
            }
            
            start = pos + 1;
        }
        while( pos != std::string::npos );
    }
    //=============================================================================================================
    void CHelper::Split(std::vector<std::string>& out, const std::string& separators, const std::string& str, bool keepempty)
    {
        out.clear();
        out.reserve(5);

        size_t start = 0;
        size_t pos = 0;
        
        do
        {
            pos = str.find_first_of(separators.c_str(), start);

            if( keepempty || pos - start > 0 )
            {
                if( out.capacity() == out.size() )
                    out.reserve(out.size() + 5);

                out.push_back(str.substr(start, pos - start));
            }
            
            start = str.find_first_not_of(separators.c_str(), pos);
        }
        while( pos != std::string::npos );
    }
    //=============================================================================================================
    std::string& CHelper::GetExtension(std::string &out, const std::string& str)
    {
        size_t pos = str.find_last_of(".");

        if( pos == std::string::npos )
        {
            out = "";
            return out;
        }

        out = str.substr(pos + 1, str.length());
        return out;
    }
    //=============================================================================================================
    std::string& CHelper::GetFile(std::string &out, const std::string& str)
    {
        size_t pos = str.find_last_of("\\/");
        out = str.substr(pos + 1);

        return out;
    }
    //=============================================================================================================
    std::string& CHelper::GetName(std::string &out, const std::string& str)
    {
        GetFile(out, str);
        size_t pos = out.find(".");

        out = out.substr(0, pos);
        return out;
    }
    //=============================================================================================================
    std::string& CHelper::GetPath(std::string &out, const std::string& str)
    {
        size_t pos = str.find_last_of("\\/");

        if( pos == std::string::npos )
        {
            out = "";
            return out;
        }

        out = str.substr(0, pos) + '/';
        return out;
    }
    //=============================================================================================================
    std::string& CHelper::Crop(std::string& out, const std::string& str, char ch)
    {
        size_t i = 0;
        size_t j = str.length() - 1;

        for( ; str[i] == ch; ++i )
            ;

        for( ; str[j] == ch; --j )
            ;

		if( j >= i )
			out = str.substr(i, (j - i) + 1);

        return out;
    }
    //=============================================================================================================
    std::string& CHelper::Replace(std::string& out, const std::string& what, const std::string& with, const std::string& str)
    {
        size_t pos = str.find(what);
        out = str;

        while( pos != std::string::npos )
        {
            out.replace(pos, what.length(), with);
            pos = out.find(what, pos + with.length());
        }

        return out;
    }
    //=============================================================================================================
	std::string& CHelper::Replace(std::string& out, char what, char with, const std::string& str)
	{
		out.resize(str.length());

		for( size_t i = 0; i < str.length(); ++i )
		{
			if( str[i] == what )
				out[i] = with;
			else
				out[i] = str[i];
		}

		return out;
	}
    //=============================================================================================================
    #define CASEENTRY(x) case x: return #x;

    std::string CHelper::FormatToString(D3DFORMAT format)
    {
        switch( format )
        {
            CASEENTRY(D3DFMT_A8R8G8B8);
            CASEENTRY(D3DFMT_X8R8G8B8);
            CASEENTRY(D3DFMT_A1R5G5B5);
            CASEENTRY(D3DFMT_X1R5G5B5);
            CASEENTRY(D3DFMT_R5G6B5);
            CASEENTRY(D3DFMT_D24S8);
            CASEENTRY(D3DFMT_D24X8);
            CASEENTRY(D3DFMT_D24X4S4);
            CASEENTRY(D3DFMT_D15S1);
            CASEENTRY(D3DFMT_D32);
            CASEENTRY(D3DFMT_D16);

        default:
            break;
        }

        return "D3DFMT_UNKNOWN";
    }
    //=============================================================================================================
}
