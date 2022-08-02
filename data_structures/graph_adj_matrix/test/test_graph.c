#include <graph_adj_matrix.h>
#include <assert.h>
#include <stdlib.h>

static void graph_create_test(void);
static void graph_update_vertex_1_test(void);
static void graph_update_vertex_multi_test(void);
static void graph_update_edge_1_test(void);
static void graph_update_edge_multi_test(void);
static void graph_update_edge_symmetric_1_test(void);
static void graph_update_edge_symmetric_multi_test(void);
static void graph_update_vertices_from_array_test(void);
static void graph_update_edges_from_array_test(void);
static void graph_copy_vertices_test(void);
static void graph_vertex_neighbours_test(void);
static void graph_copy_test(void);
static void graph_complement_test(void);

static void graph_create_test(void)
{
  {
    // Invalid argument/s - invalid no_vertices
    Graph* g = graph_create(0);
    assert(g == NULL);
    assert(graph_get_no_vertices(g) == 0);
    assert(graph_get_no_edges(g) == 0);

    graph_destroy(g);
  }
  {
    // Valid argument/s
    register const size_t no_vertices = 2;
    Graph *g = graph_create(no_vertices);
    assert(g != NULL);
    assert(graph_get_no_vertices(g) == no_vertices);
    assert(graph_get_no_edges(g) == 0);

    // Check whether values are garbage, they should be equal to 0
    for (size_t i = 0; i < no_vertices; i++)
      assert(graph_get_vertex_weight(g, i) == GRAPH_WEIGHT_ZERO);

    for (size_t i = 0; i < no_vertices; i++)
      for (size_t j = 0; j < no_vertices; j++)
        assert(graph_get_edge_weight(g, i, j) == GRAPH_WEIGHT_ZERO);

    graph_destroy(g);
  }
}

static void graph_update_vertex_1_test(void)
{
  {
    // Invalid argument/s - invalid graph
    Graph* g = graph_create(0);
    assert(g == NULL);
    assert(graph_get_no_vertices(g) == 0);
    assert(graph_get_no_edges(g) == 0);

    int result = graph_update_vertex(g, 0, GRAPH_WEIGHT(1));
    assert(result == -1);

    graph_destroy(g);
  }
  {
    // Invalid argument/s - id out of bounds
    register const size_t no_vertices = 1;
    Graph* g = graph_create(no_vertices);
    assert(g != NULL);
    assert(graph_get_no_vertices(g) == no_vertices);
    assert(graph_get_no_edges(g) == 0);

    int result = graph_update_vertex(g, no_vertices, GRAPH_WEIGHT(1));
    assert(result == -1);
    
    graph_destroy(g);
  }
  {
    // Valid argument/s
    register const size_t no_vertices = 1;
    register const graph_weight_t new_weight = GRAPH_WEIGHT(1);
    Graph* g = graph_create(no_vertices);
    assert(g != NULL);
    assert(graph_get_no_vertices(g) == no_vertices);
    assert(graph_get_no_edges(g) == 0);

    int result = graph_update_vertex(g, 0, new_weight);
    assert(result == 0);
    
    assert(graph_get_vertex_weight(g, 0) == new_weight);

    graph_destroy(g);
  }
}

static void graph_update_vertex_multi_test(void)
{
  {
    // Valid argument/s
    register const size_t no_vertices = 5;
    Graph* g = graph_create(no_vertices);
    assert(g != NULL);
    assert(graph_get_no_vertices(g) == no_vertices);
    assert(graph_get_no_edges(g) == 0);

    int result = -2;

    for (size_t i = 0; i < no_vertices; i++)
    {
      register const graph_weight_t new_weight = GRAPH_WEIGHT(i + 1);
      result = graph_update_vertex(g, i, new_weight);
      assert(result == 0);
    }

    for (size_t i = 0; i < no_vertices; i++)
    {
      register const graph_weight_t new_weight = GRAPH_WEIGHT(i + 1);
      assert(graph_get_vertex_weight(g, i) == new_weight);
    }
    
    graph_destroy(g);
  }
}

static void graph_update_edge_1_test(void)
{
  {
    // Invalid argument/s - invalid graph
    Graph* g = graph_create(0);
    assert(g == NULL);
    assert(graph_get_no_vertices(g) == 0);
    assert(graph_get_no_edges(g) == 0);

    int result = graph_update_edge(g, 0, 0, GRAPH_WEIGHT(1));
    assert(result == -1);

    graph_destroy(g);
  }
  {
    // Invalid argument/s - src/dest out of bounds
    register const size_t no_vertices = 1;
    Graph* g = graph_create(no_vertices);
    assert(g != NULL);
    assert(graph_get_no_vertices(g) == no_vertices);
    assert(graph_get_no_edges(g) == 0);

    int result = graph_update_edge(g, no_vertices, 0, GRAPH_WEIGHT(1));
    assert(result == -1);
    
    result = graph_update_edge(g, 0, no_vertices, GRAPH_WEIGHT(1));
    assert(result == -1);
    
    graph_destroy(g);
  }
  {
    // Valid argument/s
    register const size_t no_vertices = 1;
    register const graph_weight_t new_weight = GRAPH_WEIGHT(1);
    Graph* g = graph_create(no_vertices);
    assert(g != NULL);
    assert(graph_get_no_vertices(g) == no_vertices);
    assert(graph_get_no_edges(g) == 0);

    int result = graph_update_edge(g, 0, 0, new_weight);
    assert(result == 0);
    
    assert(graph_get_edge_weight(g, 0, 0) == new_weight);

    graph_destroy(g);
  }
}

static void graph_update_edge_multi_test(void)
{
  {
    // Valid argument/s
    register const size_t no_vertices = 5;
    Graph* g = graph_create(no_vertices);
    assert(g != NULL);
    assert(graph_get_no_vertices(g) == no_vertices);
    assert(graph_get_no_edges(g) == 0);

    int result = -2;

    for (size_t i = 0; i < no_vertices; i++)
      for (size_t j = 0; j < no_vertices; j++)
      {
        if (i == j) continue;
        register const graph_weight_t new_weight = GRAPH_WEIGHT(i + 1 + j * 10);
        result = graph_update_edge(g, i, j, new_weight);
        assert(result == 0);
      }

    for (size_t i = 0; i < no_vertices; i++)
      for (size_t j = 0; j < no_vertices; j++)
      {
        register const graph_weight_t new_weight = GRAPH_WEIGHT(i + 1 + j * 10);
        assert(graph_get_edge_weight(g, i, j) == ((i == j) ? GRAPH_WEIGHT_ZERO : new_weight));
      }
    
    graph_destroy(g);
  }
}

static void graph_update_edge_symmetric_1_test(void)
{
  {
    // Invalid argument/s - invalid graph
    Graph* g = graph_create(0);
    assert(g == NULL);
    assert(graph_get_no_vertices(g) == 0);
    assert(graph_get_no_edges(g) == 0);

    int result = graph_update_edge_symmetric(g, 0, 0, GRAPH_WEIGHT(1));
    assert(result == -1);

    graph_destroy(g);
  }
  {
    // Invalid argument/s - src/dest out of bounds
    register const size_t no_vertices = 1;
    Graph* g = graph_create(no_vertices);
    assert(g != NULL);
    assert(graph_get_no_vertices(g) == no_vertices);
    assert(graph_get_no_edges(g) == 0);

    int result = graph_update_edge_symmetric(g, no_vertices, 0, GRAPH_WEIGHT(1));
    assert(result == -1);
    
    result = graph_update_edge_symmetric(g, 0, no_vertices, GRAPH_WEIGHT(1));
    assert(result == -1);
    
    graph_destroy(g);
  }
  {
    // Invalid argument/s - invalid weight
    register const size_t no_vertices = 1;
    Graph* g = graph_create(no_vertices);
    assert(g != NULL);
    assert(graph_get_no_vertices(g) == no_vertices);
    assert(graph_get_no_edges(g) == 0);

    int result = graph_update_edge_symmetric(g, 0, 0, GRAPH_WEIGHT_ZERO);
    assert(result == -1);
    
    graph_destroy(g);
  }
  {
    // Valid argument/s
    register const size_t no_vertices = 1;
    register const graph_weight_t new_weight = GRAPH_WEIGHT(1);
    Graph* g = graph_create(no_vertices);
    assert(g != NULL);
    assert(graph_get_no_vertices(g) == no_vertices);
    assert(graph_get_no_edges(g) == 0);

    int result = graph_update_edge_symmetric(g, 0, 0, new_weight);
    assert(result == 0);
    
    assert(graph_get_edge_weight(g, 0, 0) == new_weight);

    graph_destroy(g);
  }
}

static void graph_update_edge_symmetric_multi_test(void)
{
  {
    // Valid argument/s
    register const size_t no_vertices = 5;
    Graph* g = graph_create(no_vertices);
    assert(g != NULL);
    assert(graph_get_no_vertices(g) == no_vertices);
    assert(graph_get_no_edges(g) == 0);

    int result = -2;

    for (size_t i = 0; i < no_vertices; i++)
      for (size_t j = i + 1; j < no_vertices; j++)
      {
        register const graph_weight_t new_weight = GRAPH_WEIGHT(i + 1 + j * 10);
        result = graph_update_edge_symmetric(g, i, j, new_weight);
        assert(result == 0);
      }

    for (size_t i = 0; i < no_vertices; i++)
    {
      for (size_t j = i; j < no_vertices; j++)
      {
        register const graph_weight_t new_weight = GRAPH_WEIGHT(i + 1 + j * 10);
        assert(graph_get_edge_weight(g, i, j) == ((i == j) ? GRAPH_WEIGHT_ZERO : new_weight));
        assert(graph_get_edge_weight(g, i, j) == graph_get_edge_weight(g, j, i));
      }
    }
    
    graph_destroy(g);
  }
}

static void graph_update_vertices_from_array_test(void)
{
  {
    // Invalid argument/s - invalid graph
    register const size_t no_vertices = 0;
    register const size_t no_updated_vertices = 3;
    const Vertex updated_vertices[] = { 
      (Vertex){.id = 0, .weight = GRAPH_WEIGHT(1)},
      (Vertex){.id = 1, .weight = GRAPH_WEIGHT(2)},
      (Vertex){.id = 2, .weight = GRAPH_WEIGHT(3)},
    };
    Graph* g = graph_create(no_vertices);
    assert(g == NULL);
    assert(graph_get_no_vertices(g) == no_vertices);
    assert(graph_get_no_edges(g) == 0);        

    int result = graph_update_vertices_from_array(g, updated_vertices, no_updated_vertices);
    assert(result == -1);
    
    graph_destroy(g);
  }
  {
    // Invalid argument/s - invalid vertices array
    register const size_t no_vertices = 3;
    register const size_t no_updated_vertices = 3;
    const Vertex* updated_vertices = NULL;
    Graph* g = graph_create(no_vertices);
    assert(g != NULL);
    assert(graph_get_no_vertices(g) == no_vertices);
    assert(graph_get_no_edges(g) == 0);        

    int result = graph_update_vertices_from_array(g, updated_vertices, no_updated_vertices);
    assert(result == -1);
    
    graph_destroy(g);  
  }
  {
    // Invalid argument/s - invalid updated vertices number
    register const size_t no_vertices = 3;
    register const size_t no_updated_vertices = 0;
    const Vertex updated_vertices[] = { 
      (Vertex){.id = 0, .weight = GRAPH_WEIGHT(1)},
      (Vertex){.id = 1, .weight = GRAPH_WEIGHT(2)},
      (Vertex){.id = 2, .weight = GRAPH_WEIGHT(3)},
    };
    Graph* g = graph_create(no_vertices);
    assert(g != NULL);
    assert(graph_get_no_vertices(g) == no_vertices);
    assert(graph_get_no_edges(g) == 0);        

    int result = graph_update_vertices_from_array(g, updated_vertices, no_updated_vertices);
    assert(result == -1);
    
    graph_destroy(g);  
  }
  {
    // Valid argument/s
    register const size_t no_vertices = 3;
    register const size_t no_updated_vertices = 3;
    const Vertex updated_vertices[] = { 
      (Vertex){.id = 0, .weight = GRAPH_WEIGHT(1)},
      (Vertex){.id = 1, .weight = GRAPH_WEIGHT(2)},
      (Vertex){.id = 2, .weight = GRAPH_WEIGHT(3)},
    };
    Graph* g = graph_create(no_vertices);
    assert(g != NULL);
    assert(graph_get_no_vertices(g) == no_vertices);
    assert(graph_get_no_edges(g) == 0);        

    int result = graph_update_vertices_from_array(g, updated_vertices, no_updated_vertices);
    assert(result == 0);
    
    graph_destroy(g);
  }
  {
    // Check if proper error index is returned - invalid id
    register const size_t no_vertices = 3;
    register const size_t no_updated_vertices = 3;
    register const size_t expected_failure = 1;
    const Vertex updated_vertices[] = { 
      (Vertex){.id = 0, .weight = GRAPH_WEIGHT(1)},
      (Vertex){.id = no_vertices, .weight = GRAPH_WEIGHT(2)}, // Should be invalid
      (Vertex){.id = 2, .weight = GRAPH_WEIGHT(3)},
    };
    Graph* g = graph_create(no_vertices);
    assert(g != NULL);
    assert(graph_get_no_vertices(g) == no_vertices);
    assert(graph_get_no_edges(g) == 0);        

    int result = graph_update_vertices_from_array(g, updated_vertices, no_updated_vertices);
    assert(result < -1 && GRAPH_ARRAY_UPDATE_ERROR_INDEX(result) == expected_failure);
    
    graph_destroy(g);
  }
}

static void graph_update_edges_from_array_test(void)
{
  {
    // Invalid argument/s - invalid graph
    register const size_t no_vertices = 0;
    register const size_t no_updated_edges = 3;
    const Edge updated_edges[] = { 
      (Edge){.src = 0, .dest = 1, .weight = GRAPH_WEIGHT(1)},
      (Edge){.src = 1, .dest = 2, .weight = GRAPH_WEIGHT(2)},
      (Edge){.src = 2, .dest = 0, .weight = GRAPH_WEIGHT(3)},
    };
    Graph* g = graph_create(no_vertices);
    assert(g == NULL);
    assert(graph_get_no_vertices(g) == no_vertices);
    assert(graph_get_no_edges(g) == 0);        

    int result = graph_update_edges_from_array(g, updated_edges, no_updated_edges);
    assert(result == -1);
    
    graph_destroy(g);
  }
  {
    // Invalid argument/s - invalid edges array
    register const size_t no_vertices = 3;
    register const size_t no_updated_edges = 3;
    const Edge* updated_edges = NULL;
    Graph* g = graph_create(no_vertices);
    assert(g != NULL);
    assert(graph_get_no_vertices(g) == no_vertices);
    assert(graph_get_no_edges(g) == 0);        

    int result = graph_update_edges_from_array(g, updated_edges, no_updated_edges);
    assert(result == -1);
    
    graph_destroy(g);
  }
  {
    // Invalid argument/s - invalid updated edges number
    register const size_t no_vertices = 3;
    register const size_t no_updated_edges = 0;
    const Edge updated_edges[] = {
      (Edge){.src = 0, .dest = 1, .weight = GRAPH_WEIGHT(1)},
      (Edge){.src = 1, .dest = 2, .weight = GRAPH_WEIGHT(2)},
      (Edge){.src = 2, .dest = 0, .weight = GRAPH_WEIGHT(3)},
    };
    Graph* g = graph_create(no_vertices);
    assert(g != NULL);
    assert(graph_get_no_vertices(g) == no_vertices);
    assert(graph_get_no_edges(g) == 0);

    int result = graph_update_edges_from_array(g, updated_edges, no_updated_edges);
    assert(result == -1);
    
    graph_destroy(g);
  }
  {
    // Valid argument/s
    register const size_t no_vertices = 3;
    register const size_t no_updated_edges = 6;
    register const graph_weight_t default_weight = GRAPH_WEIGHT(4);
    enum { ALL_POSSIBLE_EDGES = 6 };
    Edge updated_edges[ALL_POSSIBLE_EDGES] = { (Edge){} };
    size_t counter = 0;
    for (size_t i = 0; i < 3; i++)
      for (size_t j = i; j < 3; j++)
      {
        updated_edges[counter] = (Edge){.src = i, .dest = j, .weight = GRAPH_WEIGHT(i * 10 + j + 1)};
        counter++;
      }

    Graph* g = graph_create(no_vertices);
    assert(g != NULL);
    assert(graph_get_no_vertices(g) == no_vertices);
    assert(graph_get_no_edges(g) == 0); 

    for (size_t i = 0; i < 3; i++)
      for (size_t j = 0; j < 3; j++)
      {
        int result = graph_add_edge(g, i, j, default_weight);     
        assert(result == 0);
        assert(graph_get_edge_weight(g, i, j) == default_weight);
      }

    int result = graph_update_edges_from_array(g, updated_edges, no_updated_edges);
    assert(result == 0);

    for (size_t i = 0; i < 3; i++)
      for (size_t j = i; j < 3; j++)
        assert(graph_get_edge_weight(g, i, j) == GRAPH_WEIGHT(i * 10 + j + 1));
    
    graph_destroy(g);
  }
  {
    // Check if proper error index is returned - invalid src or dest
    register const size_t no_vertices = 3;
    register const size_t no_updated_edges = 3;
    register const size_t expected_failure = 1;
    register const graph_weight_t default_weight = GRAPH_WEIGHT(4);    
    const Edge updated_edges[] = {
      (Edge){.src = 0, .dest = 1, .weight = default_weight},
      (Edge){.src = no_vertices, .dest = no_vertices, .weight = default_weight}, // Should be invalid
      (Edge){.src = 2, .dest = 0, .weight = default_weight},
    };
    Graph* g = graph_create(no_vertices);
    assert(g != NULL);
    assert(graph_get_no_vertices(g) == no_vertices);
    assert(graph_get_no_edges(g) == 0);

    for (size_t i = 0; i < no_vertices; i++)
    {
      int result = graph_add_edge(g, i, (i + 1) % no_vertices, GRAPH_WEIGHT(4));
      assert(result == 0);
      assert(graph_get_edge_weight(g, i, (i + 1) % no_vertices) == default_weight);
    }
      
    int result = graph_update_edges_from_array(g, updated_edges, no_updated_edges);
    assert(result < -1 && GRAPH_ARRAY_UPDATE_ERROR_INDEX(result) == expected_failure);
    
    graph_destroy(g);
  }
}

static void graph_add_vertice_test(void)
{
  {
    // Invalid argument/s - invalid graph
    register const size_t no_vertices = 0;
    register const graph_weight_t new_weight = GRAPH_WEIGHT(42);
    Graph* g = graph_create(no_vertices);
    assert(g == NULL);
    assert(graph_get_no_vertices(g) == no_vertices);
    assert(graph_get_no_edges(g) == 0);        

    int result = graph_add_vertex(g, new_weight);
    assert(result == -1);
    
    graph_destroy(g);
  }
  {
    // Valid argument/s
    register const size_t no_vertices = 1;
    register const graph_weight_t new_weight = GRAPH_WEIGHT(42);
    Graph* g = graph_create(no_vertices);
    assert(g != NULL);
    assert(graph_get_no_vertices(g) == no_vertices);
    assert(graph_get_no_edges(g) == 0);        

    int result = graph_add_vertex(g, new_weight);
    assert(result == 0);
    assert(graph_get_no_vertices(g) == no_vertices + 1);
    assert(graph_get_no_edges(g) == 0);

    graph_destroy(g);
  }
}

static void graph_add_edge_test(void)
{
  {
    // Invalid argument/s - invalid graph
    register const size_t no_vertices = 0;
    register const graph_weight_t new_weight = GRAPH_WEIGHT(42);
    Graph* g = graph_create(no_vertices);
    assert(g == NULL);
    assert(graph_get_no_vertices(g) == no_vertices);
    assert(graph_get_no_edges(g) == 0);        

    int result = graph_add_edge(g, 0, 0, new_weight);
    assert(result == -1);
    
    graph_destroy(g);
  }
  {
    // Invalid argument/s - invalid weight
    register const size_t no_vertices = 1;
    register const graph_weight_t new_weight = GRAPH_WEIGHT_ZERO;
    Graph* g = graph_create(no_vertices);
    assert(g != NULL);
    assert(graph_get_no_vertices(g) == no_vertices);
    assert(graph_get_no_edges(g) == 0);        

    int result = graph_add_edge(g, 0, 0, new_weight);
    assert(result == -1);
    
    graph_destroy(g);
  }
  {
    // Valid argument/s
    register const size_t no_vertices = 1;
    register const graph_weight_t new_weight = GRAPH_WEIGHT(42);
    Graph* g = graph_create(no_vertices);
    assert(g != NULL);
    assert(graph_get_no_vertices(g) == no_vertices);
    assert(graph_get_no_edges(g) == 0);        

    int result = graph_add_edge(g, 0, 0, new_weight);
    assert(result == 0);
    assert(graph_get_no_vertices(g) == no_vertices);
    assert(graph_get_no_edges(g) == 1);
    
    graph_destroy(g);
  }
}

static void graph_copy_vertices_test(void)
{
  {
    // Invalid argument/s - invalid graph
    register const size_t no_vertices = 0;
    Graph* g = graph_create(no_vertices);
    assert(g == NULL);
    assert(graph_get_no_vertices(g) == 0);
    assert(graph_get_no_edges(g) == 0);        

    Vector* copied_vertices = graph_copy_vertices(g);
    assert(copied_vertices == NULL);

    vector_destroy(copied_vertices);
    graph_destroy(g);
  }
  {
    // Valid argument/s
    register const size_t no_vertices = 3;
    register const size_t no_updated_vertices = 3;
    const Vertex updated_vertices[] = { 
      (Vertex){.id = 0, .weight = GRAPH_WEIGHT(1)},
      (Vertex){.id = 1, .weight = GRAPH_WEIGHT(2)},
      (Vertex){.id = 2, .weight = GRAPH_WEIGHT(3)},
    };
    Graph* g = graph_create(no_vertices);
    assert(g != NULL);
    assert(graph_get_no_vertices(g) == no_vertices);
    assert(graph_get_no_edges(g) == 0);        

    int result = graph_update_vertices_from_array(g, updated_vertices, no_updated_vertices);
    assert(result == 0);
    
    Vector* copied_vertices = graph_copy_vertices(g);
    assert(copied_vertices != NULL);

    Vertex* vert = malloc(sizeof(*vert));

    for (size_t i = 0; i < vector_get_curr_size(copied_vertices); i++)
    {
      result = vector_at(copied_vertices, i, vert);
      assert(result == 0);
      assert(vert->weight == graph_get_vertex_weight(g, i));
    }

    free(vert);
    vector_destroy(copied_vertices);
    graph_destroy(g);
  }
}

static void graph_vertex_neighbours_test(void)
{
  {
    // Invalid argument/s - invalid graph
    register const size_t no_vertices = 0;
    register const size_t id = 0;
    Graph* g = graph_create(no_vertices);
    assert(g == NULL);
    assert(graph_get_no_vertices(g) == no_vertices);
    assert(graph_get_no_edges(g) == 0);        

    Vector* vert_neighbours = graph_vertex_neighbours(g, id);
    assert(vert_neighbours == NULL);

    vector_destroy(vert_neighbours);
    graph_destroy(g);   
  }
  {
    // Invalid argument/s - invalid vertex
    register const size_t no_vertices = 3;
    register const size_t id = 3;
    Graph* g = graph_create(no_vertices);
    assert(g != NULL);
    assert(graph_get_no_vertices(g) == no_vertices);
    assert(graph_get_no_edges(g) == 0);        

    Vector* vert_neighbours = graph_vertex_neighbours(g, id);
    assert(vert_neighbours == NULL);

    vector_destroy(vert_neighbours);
    graph_destroy(g);   
  }
  {
    // Valid argument/s
    register const size_t no_vertices = 3;
    register const size_t id = 0;
    Graph* g = graph_create(no_vertices);
    assert(g != NULL);
    assert(graph_get_no_vertices(g) == no_vertices);
    assert(graph_get_no_edges(g) == 0);        

    for (size_t i = 0; i < no_vertices; i++)
    {
      int result = graph_add_edge(g, id, i, GRAPH_WEIGHT(i + 1));
      assert(result == 0);
    }

    assert(graph_get_no_edges(g) == no_vertices);

    Vector* vert_neighbours = graph_vertex_neighbours(g, id);
    assert(vert_neighbours != NULL);
    assert(vector_get_curr_size(vert_neighbours) == no_vertices);

    Edge* edge = malloc(sizeof(*edge));
    for (size_t i = 0; i < no_vertices; i++)
    {
      int result = vector_at(vert_neighbours, i, edge);
      assert(result == 0);
      assert(edge->src == id && edge->dest == i);
      assert(edge->weight == GRAPH_WEIGHT(i + 1));
      assert(edge->weight == graph_get_edge_weight(g, id, i));
    }

    free(edge);
    vector_destroy(vert_neighbours);
    graph_destroy(g);   
  }  
}

static void graph_copy_test(void)
{
  {
    // Invalid argument/s - invalid graph
    register const size_t no_vertices = 0;
    Graph* g = graph_create(no_vertices);
    assert(g == NULL);
    assert(graph_get_no_vertices(g) == no_vertices);
    assert(graph_get_no_edges(g) == 0);        

    Graph* g_copy = graph_copy(g);
    assert(g_copy == NULL);

    graph_destroy(g);
    graph_destroy(g_copy);   
  }
  {
    // Valid argument/s
    register const size_t no_vertices = 3;
    register const size_t id = 0;
    Graph* g = graph_create(no_vertices);
    assert(g != NULL);
    assert(graph_get_no_vertices(g) == no_vertices);
    assert(graph_get_no_edges(g) == 0);        

    for (size_t i = 0; i < no_vertices; i++)
    {
      int result = graph_add_edge(g, id, i, GRAPH_WEIGHT(i + 1));
      assert(result == 0);
    }

    assert(graph_get_no_edges(g) == no_vertices);

    Graph* g_copy = graph_copy(g);
    assert(g_copy != NULL);
    assert(graph_get_no_edges(g) == graph_get_no_edges(g_copy));
    assert(graph_get_no_vertices(g) == graph_get_no_vertices(g_copy));

    for (size_t i = 0; i < no_vertices; i++)
    {
      assert(graph_get_vertex_weight(g, i) == graph_get_vertex_weight(g_copy, i));
      assert(graph_get_edge_weight(g, id, i) == graph_get_edge_weight(g_copy, id, i));
    }

    graph_destroy(g);
    graph_destroy(g_copy);
  }
}

#define ALL_POSSIBLE_EDGES(no_vertices) \
  (no_vertices) * ((no_vertices) - 1)

static void graph_complement_test(void)
{
  {
    // Invalid argument/s - invalid graph
    register const size_t no_vertices = 0;
    Graph* g = graph_create(no_vertices);
    assert(g == NULL);
    assert(graph_get_no_vertices(g) == no_vertices);
    assert(graph_get_no_edges(g) == 0);        

    Graph* g_complement = graph_complement(g);
    assert(g_complement == NULL);

    graph_destroy(g);
    graph_destroy(g_complement);   
  }
  {
    // Valid argument/s - complete graph
    register const size_t no_vertices = 3;
    Graph* g = graph_create(no_vertices);
    assert(g != NULL);
    assert(graph_get_no_vertices(g) == no_vertices);
    assert(graph_get_no_edges(g) == 0);        

    for (size_t i = 0; i < no_vertices; i++)
      for (size_t j = 0; j < no_vertices; j++)
      {
        if (i == j)
          continue;
        
        int result = graph_add_edge(g, i, j, GRAPH_WEIGHT(1));
        assert(result == 0);
      }

    assert(graph_get_no_edges(g) == ALL_POSSIBLE_EDGES(no_vertices));

    Graph* g_complement = graph_complement(g);
    assert(g_complement != NULL);
    assert(graph_get_no_edges(g_complement) == ALL_POSSIBLE_EDGES(no_vertices) - graph_get_no_edges(g));
    assert(graph_get_no_vertices(g) == graph_get_no_vertices(g_complement));

    for (size_t i = 0; i < no_vertices; i++)
    {
      assert(graph_get_vertex_weight(g, i) == graph_get_vertex_weight(g_complement, i));
      for (size_t j = 0; j < no_vertices; j++)
      {
        assert(graph_get_edge_weight(g_complement, i, j) == GRAPH_WEIGHT_ZERO);
      }
    }

    graph_destroy(g);
    graph_destroy(g_complement);
  }
  {
    // Valid argument/s - empty graph
    register const size_t no_vertices = 3;
    Graph* g = graph_create(no_vertices);
    assert(g != NULL);
    assert(graph_get_no_vertices(g) == no_vertices);
    assert(graph_get_no_edges(g) == 0);        

    assert(graph_get_no_edges(g) == 0);

    Graph* g_complement = graph_complement(g);
    assert(g_complement != NULL);
    assert(graph_get_no_edges(g_complement) == ALL_POSSIBLE_EDGES(no_vertices) - graph_get_no_edges(g));
    assert(graph_get_no_vertices(g) == graph_get_no_vertices(g_complement));

    for (size_t i = 0; i < no_vertices; i++)
    {
      assert(graph_get_vertex_weight(g, i) == graph_get_vertex_weight(g_complement, i));
      for (size_t j = 0; j < no_vertices; j++)
      {
        assert(graph_get_edge_weight(g_complement, i, j) == ((i == j) ? GRAPH_WEIGHT_ZERO : GRAPH_WEIGHT(1)));
      }
    }

    graph_destroy(g);
    graph_destroy(g_complement);
  }
  {
    // Valid argument/s - complete graph
    register const size_t no_vertices = 3;
    register const size_t no_edges = 3;
    Graph* g = graph_create(no_vertices);
    assert(g != NULL);
    assert(graph_get_no_vertices(g) == no_vertices);
    assert(graph_get_no_edges(g) == 0);        

    for (size_t i = 0; i < no_vertices; i++)
      for (size_t j = i + 1; j < no_vertices; j++)
      {
        int result = graph_add_edge(g, i, j, GRAPH_WEIGHT(1));
        assert(result == 0);
      }

    assert(graph_get_no_edges(g) == no_edges);

    Graph* g_complement = graph_complement(g);
    assert(g_complement != NULL);
    assert(graph_get_no_edges(g_complement) == ALL_POSSIBLE_EDGES(no_vertices) - no_edges);
    assert(graph_get_no_vertices(g) == graph_get_no_vertices(g_complement));

    for (size_t i = 0; i < no_vertices; i++)
    {
      assert(graph_get_vertex_weight(g, i) == graph_get_vertex_weight(g_complement, i));
      for (size_t j = 0; j < no_vertices; j++)
      {
        assert(graph_get_edge_weight(g_complement, i, j) == ((i > j) ? GRAPH_WEIGHT(1) : GRAPH_WEIGHT_ZERO));
      }
    }

    graph_destroy(g);
    graph_destroy(g_complement);
  }
}

void graph_main_test(void)
{
  // Create
  graph_create_test();
  // Update vertex
  graph_update_vertex_1_test();
  graph_update_vertex_multi_test();
  // Update edge
  graph_update_edge_1_test();
  graph_update_edge_multi_test();
  // Update edge symmetric
  graph_update_edge_symmetric_1_test();
  graph_update_edge_symmetric_multi_test();
  // Update from arrays
  graph_update_vertices_from_array_test();
  graph_update_edges_from_array_test();
  // Add vertex/add edge
  graph_add_vertice_test();
  graph_add_edge_test();
  // Copy vertices
  graph_copy_vertices_test();
  // Get vertex neighbours
  graph_vertex_neighbours_test();
  // Copy
  graph_copy_test();
  // Complement
  graph_complement_test();
}
