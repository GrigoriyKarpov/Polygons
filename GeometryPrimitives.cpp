#include <cassert>
#include <cmath>
#include <algorithm>

#include "GeometryPrimitives.h"

Segment::Segment( Point const & iPoint1, Point const & iPoint2 )
	: m_point1( iPoint1 )
	, m_point2( iPoint2 )
{}

bool Segment::Contain( Point const & iPoint ) const {
	if (
		iPoint.x < std::min( m_point1.x, m_point2.x ) ||
		iPoint.y < std::min( m_point1.y, m_point2.y ) ||
		iPoint.x > std::max( m_point1.x, m_point2.x ) ||
		iPoint.y > std::max( m_point1.y, m_point2.y )
	) {
		return false;
	}

	Vector const vector1( m_point1, iPoint );
	Vector const vector2( m_point1, m_point2 );
	return vector1.GetVectorProduct( vector2 ) == 0;
}

bool Segment::Intersect( Segment const & iOther ) const {
	// http://grafika.me/node/237

	Vector const vector1( *this );
	Vector const vector2( iOther );

	double const vp1 = vector1.GetVectorProduct( Vector( m_point1, iOther.m_point1 ) );
	double const vp2 = vector1.GetVectorProduct( Vector( m_point1, iOther.m_point2 ) );
	double const vp3 = vector2.GetVectorProduct( Vector( iOther.m_point1, m_point1 ) );
	double const vp4 = vector2.GetVectorProduct( Vector( iOther.m_point1, m_point2 ) );

	return vp1 * vp2 < 0 && vp3 * vp4 < 0;
}

bool Segment::Intersect( Segment const & iOther, Point & oIntersection ) const {
	// http://algolist.manual.ru/maths/geom/intersect/lineline2d.php

	if ( !Intersect( iOther ) ) {
		return false;
	}

	double const numerator = Vector( iOther ).GetVectorProduct( Vector( iOther.m_point1, m_point1 ) );
	double const denominator = Vector( *this ).GetVectorProduct( Vector( iOther ) );

	if( denominator == 0 ){
		return false;
	}

	double const t = numerator / denominator;
	oIntersection.x = m_point1.x + t * ( m_point2.x - m_point1.x );
	oIntersection.y = m_point1.y + t * ( m_point2.y - m_point1.y );

	return true;
}

Vector::Vector( Segment const & iSegment )
    : Vector( iSegment.m_point1, iSegment.m_point2 )
{}

double Vector::GetLength() const {
	double const product = std::sqrt( GetScalarProduct( *this ) );
	assert( product != 0 && "Vector has 0 length." );
	return product;
}
