#ifndef __GeometryPrimitives_H_
#define __GeometryPrimitives_H_

#include <vector>

struct Point {
	Point(): x( 0 ), y( 0 ) {}
	Point( double iX, double iY ): x( iX ), y( iY ) {}

	bool operator == ( Point const & iOther ) const { return x == iOther.x && y == iOther.y; }
	bool operator != ( Point const & iOther ) const { return !(iOther == *this); }

	double x;
	double y;
};

struct Segment {
	Segment() {}
	Segment( Point const & iPoint1, Point const & iPoint2 );

	bool Contain( Point const & iPoint ) const;
	bool Intersect( Segment const & iOther ) const;
	bool Intersect( Segment const & iOther, Point & oIntersection ) const;

	Segment Revert() const { return Segment( m_point2, m_point1 ); }

	Point m_point1;
	Point m_point2;
};

class Polyline {
public:
	Polyline() {}
	Polyline( std::vector< Point > iPoints ): m_points( iPoints ) {}

	std::vector< Point > GetVertexes() const { return m_points; }

	void AddPoint( Point const & iPoint ) { m_points.push_back( iPoint ); }

private:
	std::vector< Point > m_points;
};

struct Vector {
	Vector() {}
	Vector( double iDX, double iDY ): m_direction( iDX, iDY ) {}
	Vector( Point const & iBegin, Point const & iEnd )
		: m_direction( iEnd.x - iBegin.x, iEnd.y - iBegin.y )
	{}
	explicit Vector( Segment const & iSegment );

	double GetDX() const { return m_direction.x; }
	double GetDY() const { return m_direction.y; }
	double GetLength() const;

	double GetCos( Vector const & iOther ) const {
		return GetScalarProduct( iOther ) / ( GetLength() * iOther.GetLength() );
	}

	double GetSin( Vector const & iOther ) const {
		return GetVectorProduct( iOther ) / ( GetLength() * iOther.GetLength() );
	}

	double GetScalarProduct( Vector const & iOther ) const {
		return m_direction.x * iOther.m_direction.x + m_direction.y * iOther.m_direction.y;
	}

	double GetVectorProduct( Vector const & iOther ) const {
		return m_direction.x * iOther.m_direction.y - m_direction.y * iOther.m_direction.x;
	}

private:
	Point m_direction;
};

#endif // __GeometryPrimitives_H_
