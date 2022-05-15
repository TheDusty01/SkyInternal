#pragma once

#include <array>
#include <string>
#include <ostream>
#include <algorithm>

namespace Sky
{

	namespace Core
	{

#pragma region Matrix
        struct RowCol
        {
            size_t Row, Col;
        };

        template <typename Type, size_t TRowCount, size_t TColCount>
        class Matrix
        {
        public:
            struct RowCol
            {
                size_t Row, Col;
            };

            using array_t = std::array<Type, TRowCount* TColCount>;
            using matrix_t = Matrix<Type, TRowCount, TColCount >;
            using col_matrix_t = Matrix<Type, 1, TColCount>;
            using row_matrix_t = Matrix<Type, TRowCount, 1>;

        protected:
            array_t m_values;

        public:
            Matrix()
            {
                static_assert(std::is_arithmetic<Type>::value, "Type T has to be arithmetic!");
                static_assert(TRowCount >= 1 && TColCount >= 1, "Minimal row and col dimensions are 1.");
                m_values.fill(static_cast<Type>(0));
            }

            ~Matrix()
            {
            }

            explicit Matrix(array_t values)
                : m_values(values)
            {
                static_assert(std::is_arithmetic<Type>::value, "Type T has to be arithmetic!");
            }

            template <typename... TParams>
            Matrix(TParams... args)
            {
                static_assert(std::is_arithmetic<Type>::value, "Type T has to be arithmetic!");
                m_values = array_t{ static_cast<Type>(args)... };
            }

            constexpr static size_t RowCount()
            {
                return TRowCount;
            }

            constexpr static size_t ColCount()
            {
                return TColCount;
            }

            constexpr static size_t Size()
            {
                return TRowCount * TColCount;
            }

            bool IsEmpty() const
            {
                return all_of(m_values.begin(), m_values.end(), [](Type i)
                    {
                        return i == static_cast<Type>(0);
                    });
            }

            col_matrix_t Row(size_t rowIndex) const
            {
                std::array<Type, TColCount> values;

                for (size_t i = 0; i < TColCount; ++i)
                    values.at(i) = At(rowIndex, i);

                return col_matrix_t{ values };
            }

            row_matrix_t Col(size_t colIndex) const
            {
                std::array<Type, TRowCount> values;

                for (size_t i = 0; i < TRowCount; ++i)
                    values.at(i) = At(i, colIndex);

                return row_matrix_t{ values };
            }

            void Row(size_t rowIndex, col_matrix_t mat)
            {
                for (size_t i = 0; i < TColCount; ++i)
                    At(rowIndex, i) = mat(i);
            }

            void Col(size_t colIndex, row_matrix_t mat)
            {
                for (size_t i = 0; i < TRowCount; ++i)
                    At(i, colIndex) = mat(i);
            }

            Type& At(size_t index)
            {
                return m_values.at(index);
            }

            const Type& At(size_t index) const
            {
                return m_values.at(index);
            }

            Type& At(size_t rowIndex, size_t colIndex)
            {
                return m_values.at(rowIndex * TColCount + colIndex);
            }

            const Type& At(size_t rowIndex, size_t colIndex) const
            {
                return m_values.at(rowIndex * TColCount + colIndex);
            }

            Type& operator [] (size_t index)
            {
                return At(index);
            }

            const Type& operator [] (size_t index) const
            {
                return At(index);
            }

            Type& operator () (size_t index)
            {
                return At(index);
            }

            const Type& operator () (size_t index) const
            {
                return At(index);
            }

            Type& operator () (size_t rowIndex, size_t colIndex)
            {
                return At(rowIndex, colIndex);
            }

            const Type& operator () (size_t rowIndex, size_t colIndex) const
            {
                return At(rowIndex, colIndex);
            }

            Type& operator [] (RowCol rowCol)
            {
                return at(rowCol.Row, rowCol.Col);
            }

            const Type& operator [] (RowCol rowCol) const
            {
                return at(rowCol.Row, rowCol.Col);
            }

            matrix_t operator + (Type other) const
            {
                auto buf = *this;
                buf += other;
                return buf;
            }

            matrix_t operator - (Type other) const
            {
                auto buf = *this;
                buf -= other;
                return buf;
            }

            matrix_t operator * (Type other) const
            {
                auto buf = *this;
                buf *= other;
                return buf;
            }

            matrix_t operator / (Type other) const
            {
                auto buf = *this;
                buf /= other;
                return buf;
            }

            matrix_t& operator += (Type other)
            {
                for (auto& val : m_values)
                    val += other;

                return *this;
            }

            matrix_t& operator -= (Type other)
            {
                for (auto& val : m_values)
                    val -= other;

                return *this;
            }

            matrix_t& operator *= (Type other)
            {
                for (auto& val : m_values)
                    val *= other;

                return *this;
            }

            matrix_t& operator /= (Type other)
            {
                for (auto& val : m_values)
                    val /= other;

                return *this;
            }

            matrix_t operator + (const matrix_t& other) const
            {
                auto buf = *this;

                for (size_t i = 0; i < Size(); ++i)
                    buf(i) += other(i);

                return buf;
            }

            matrix_t operator - (const matrix_t& other) const
            {
                auto buf = *this;

                for (size_t i = 0; i < Size(); ++i)
                    buf(i) -= other(i);

                return buf;
            }

            matrix_t& operator += (const matrix_t& other)
            {
                for (size_t i = 0; i < Size(); ++i)
                    At(i) += other(i);

                return (*this);
            }

            bool operator == (const matrix_t& other) const
            {
                for (size_t i = 0; i < Size(); ++i)
                    if (At(i) != other.At(i))
                        return false;

                return true;
            }

            bool operator != (const matrix_t& other) const
            {
                return !((*this) == other);
            }

            matrix_t& operator -= (const matrix_t& other)
            {
                for (size_t i = 0; i < Size(); ++i)
                    At(i) -= other(i);

                return (*this);
            }

            friend std::ostream& operator <<(std::ostream& os, const matrix_t& v)
            {
                for (size_t i = 0; i < TRowCount; ++i)
                {
                    for (size_t j = 0; j < TColCount; j++)
                        os << v.At(i, j) << "\t";

                    os << "\n";
                }

                return os;
            }

        };
#pragma endregion

#pragma region Vector base
        template <typename Type, size_t TRowCount>
        class Vector : public Matrix<Type, TRowCount, 1>
        {

        public:
            using typename Matrix<Type, TRowCount, 1>::matrix_t;
            using typename matrix_t::array_t;

            // Inherit constructors
            using matrix_t::Matrix;

            Type Norm() const
            {
                Type buf = static_cast<Type>(0);

                for (auto v : this->m_cValues)
                    buf += (v * v);

                return sqrt(buf);
            }

            void Normalize()
            {
                (*this) /= this->Norm();
            }

            Vector<Type, TRowCount> ToNormalized() const
            {
                auto buf = *this;
                buf.Normalize();
                return buf;
            }

            Type Dot(const matrix_t& other) const
            {
                Type buf = static_cast<Type>(0);

                for (size_t i = 0; i < this->Size(); ++i)
                    buf += this->At(i) * other.At(i);

                return buf;
            }

        };
#pragma endregion

#pragma region Vector wrapper
        template <class Type>
        using Vector2 = Vector<Type, 2>;
        using Vector2f = Vector2<float>;

        template<class Type>
        class Vector3 : public Vector<Type, 3>
        {

        public:
            using Vector3_t = Vector<Type, 3>;

            // Inherit constructors
            using Vector3_t::Vector;

            Vector3 Cross(const Vector3& other) const
            {
                return
                {
                    this->At(1) * other.At(2) - this->At(2) * other.At(1),
                    this->At(2) * other.At(0) - this->At(0) * other.At(2),
                    this->At(0) * other.At(1) - this->At(1) * other.At(0),
                };
            }

            Vector3 NCross(const Vector3& other) const
            {
                return Cross(other).ToNormalized();
            }
        };
        using Vector3f = Vector3<float>;

        template <class Type>
        using Vector4 = Vector<Type, 4>;
        using Vector4f = Vector4<float>;
#pragma endregion


        using ViewMatrix = Matrix<float, 4, 4>;

	}

}


struct Vec2f
{
    float x, y;
};

struct Vec3f
{
    float x, y, z;
};

struct Vec4f
{
    float x, y, z, w;
};


struct Matrix4x4
{
    float data[16];
};