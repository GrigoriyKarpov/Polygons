#ifndef __Polygon_H_
#define __Polygon_H_

#include <cstddef>
#include <vector>

#include "GeometryPrimitives.h"

enum class ENormalToEdgeDirection {
    Inner, // object inner part on the left hand
    Outer  // object outer part on the right hand
};

class Polygon {
public:
	typedef std::vector< Point > Vertexes;
	typedef std::vector< Segment > Edges;
	typedef std::vector< Vector > Normals;

	// TODO: check internal part

	Polygon() {}
	explicit Polygon( Vertexes const & iVertexes ): m_vertexes( iVertexes ) {}

	void AddVertex( Point const & iPoint ) { m_vertexes.push_back( iPoint ); }

	size_t GetVertexCount() const;
	Vertexes GetVertexes() const;
	Edges GetEdges() const;
	Normals GetNormals( ENormalToEdgeDirection iNormalDirection ) const;

private:
	Vertexes m_vertexes;
};

#endif // __Polygon_H_
