#include <cassert>

#include "Polygon.h"

size_t Polygon::GetVertexCount() const {
	//assert( m_vertexes.size() > 2 && "Polygon should contain more than 2 vertices." );
	return m_vertexes.size();
}

Polygon::Vertexes Polygon::GetVertexes() const {
	GetVertexCount();
	return m_vertexes;
}

Polygon::Edges Polygon::GetEdges() const {
	size_t const size = GetVertexCount();
	size_t const last = size - 1;
	std::vector< Segment > segments( size );
	for ( size_t i = 0; i < last; ++i ) {
		segments[ i ] = Segment( m_vertexes[ i ], m_vertexes[ i + 1 ] );
	}
	segments[ last ] = Segment( m_vertexes[ last ], m_vertexes[ 0 ] );
	return segments;
}

Polygon::Normals Polygon::GetNormals( ENormalToEdgeDirection iNormalDirection ) const {
	size_t const size = GetVertexCount();
	std::vector< Segment > const & edges = GetEdges();
	std::vector< Vector > normals( size );
	for ( size_t i = 0; i < size; ++i ) {
		double const dx = edges[ i ].m_point2.x - edges[ i ].m_point1.x;
		double const dy = edges[ i ].m_point2.y - edges[ i ].m_point1.y;
        normals[ i ] = ( true )
        //normals[ i ] = ( iNormalDirection == ENormalToEdgeDirection::Inner )
			? Vector( -dy, dx )
			: Vector( dy, -dx );
	}
	return normals;
}
