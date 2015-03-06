#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <GL/freeglut.h>

// fixme:
char *GetToken(char **src);

enum
{
	POINT,
	LINE,
	TRIANGLE,
	POLYLINE,
	POLYLOOP,
	POLYGON
};

typedef struct vertex_s
{
	float xyz[3];
} vertex_t;

typedef struct polygon_s
{
	int	numvertices;
	vertex_t	*vertices;
} polygon_t;

typedef struct object_s
{
	struct object_s	*next;
	int		type;
	polygon_t	polygon;

} object_t;

// global object list
static object_t *objects;

void DrawPolygon(object_t *obj)
{
	polygon_t	*p = &obj->polygon;

	glBegin(GL_TRIANGLES);
	for(int i = 0; i < p->numvertices - 2; i++)
	{
		glVertex3f(
			p->vertices[0].xyz[0],
			p->vertices[0].xyz[1],
			p->vertices[0].xyz[2]);

		glVertex3f(
			p->vertices[i + 1].xyz[0],
			p->vertices[i + 1].xyz[1],
			p->vertices[i + 1].xyz[2]);

		glVertex3f(
			p->vertices[i + 2].xyz[0],
			p->vertices[i + 2].xyz[1],
			p->vertices[i + 2].xyz[2]);
	}
	glEnd();
}

void DrawObjects()
{
	object_t *obj = objects;
	for(; obj; obj = obj->next)
	{
		DrawPolygon(obj);
	}
}

void ExpectToken(const char *expect, char **src)
{
	char *token = GetToken(src);
	if(strcmp(expect, token))
	{
		fprintf(stderr, "Error parsing file, expected token \"%s\", got \"%s\"\n", expect, token);
		exit(1);
	}
}

void *Malloc(int numbytes)
{
	void *ptr = Malloc(numbytes);
	memset(ptr, 0, numbytes);

	return ptr;
}

object_t *MallocObject(int type)
{
	object_t *obj = (object_t*)Malloc(sizeof(object_t));
	obj->type = type;

	// link it into the global list
	obj->next = objects;
	objects = obj;

	return obj;
}

int ParseInt(char **src)
{
	char *token = GetToken(src);
	return atoi(token);
}

float ParseFloat(char **src)
{
	char *token = GetToken(src);
	return atof(token);
}

void ParsePolygon(char **src)
{
	object_t	*obj = MallocObject(POLYGON);
	polygon_t	*p = &obj->polygon;

	ExpectToken("numvertices", src);
	p->numvertices = ParseInt(src);
	p->vertices = (vertex_t*)Malloc(p->numvertices * sizeof(vertex_t));

	for(int i = 0; i < p->numvertices; i++)
	{
		ExpectToken("vertex", src);
		int n = ParseInt(src);

		p->vertices[n].xyz[0] = ParseFloat(src);
		p->vertices[n].xyz[1] = ParseFloat(src);
		p->vertices[n].xyz[2] = ParseFloat(src);
	}
}

void ParseFile(char **src)
{
	char	*token;

	while((token = GetToken(src)) != NULL)
	{
		if(!strcmp(token, "polygon"))
		{
			ParsePolygon(src);
		}
		else
		{
			fprintf(stderr, "unknown object \"%s\"\n", token);
			exit(1);
		}
	}
}

char *LoadFile(const char *filename)
{
	FILE *fp = fopen(filename, "r");

	// get the filesize
	fseek(fp, 0, SEEK_END);
	int len = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	char* buffer = (char*)malloc(len);
	fread(buffer, 1, len, fp);

	return buffer;
}

int main(int argc, char *arg[])
{
	char *buffer = LoadFile("debug_test.txt");

	ParseFile(&buffer);

	return 0;
}

#if 0
static void ReadPoint(FILE *fp)
{
	ReadVertex(fp);
}

static void ReadLine(FILE *fp)
{
	ReadVertex(fp);
	ReadVertex(fp);
}

static void ReadTriangle(FILE *fp)
{
	ReadVertex(fp);
	ReadVertex(fp);
	ReadVertex(fp);
}

static void ReadPolyLine(FILE *fp)
{
	int numvertices = ReadInt(fp);

	for(int i = 0; i < numvertices; i++)
	{
		ReadVertex(fp);
	}
}

static void ReadPolyLoop(FILE *fp)
{
	int numvertices = ReadInt(fp);
	
	for(int i = 0; i < numvertices; i++)
	{
		ReadVertex(fp);
	}

	// add 0 vertex back on
}

typedef struct point_s
{
} point_t;

typedef struct line_s
{
} line_t;

typedef struct triangle_s
{
	vertices[3];
} triangle_t;

typedef struct polyline_s
{
	vec3	*vertices;
} polyline_t;


#endif


#if 0
int ReadInt(FILE *fp)
{
	int i;

	fread(&i, 1, sizeof(int), fp);
	return i;
}

void WriteInt(int i, FILE *fp)
{
	fwrite(&i, 1, sizeof(int), fp);
}

float ReadFloat(FILE *fp)
{
	float f;

	fread(&f, 1, sizeof(float), fp);
	return f;
}

void WriteFloat(float f, FILE *fp)
{
	fwrite(&f, 1, sizeof(float), fp);
}

vertex_t ReadVertex(FILE *fp)
{
	vertex_t v;

	v.xyz[0] = ReadFloat(fp);
	v.xyz[1] = ReadFloat(fp);
	v.xyz[2] = ReadFloat(fp);

	return v;
}

void WriteVertex(vertex_t v, FILE *fp)
{
	WriteFloat(v.xyz[0], fp);
	WriteFloat(v.xyz[1], fp);
	WriteFloat(v.xyz[2], fp);
}

void WriteObjectType(int type, FILE *fp)
{
	WriteInt(type, fp);
}

void ReadPolygonBinary(object_t *obj, FILE* fp)
{
	polygon_t	*p = &obj->polygon;

	p->numvertices = ReadInt(fp);
	p->vertices = (vertex_t*)Malloc(p->numvertices * sizeof(vertex_t));
	
	for(int i = 0; i < p->numvertices; i++)
	{
		p->vertices[i] = ReadVertex(fp);
	}
}

void WritePolygonBinary(object_t *obj, FILE *fp)
{
	polygon_t	*p = &obj->polygon;

	WriteInt(p->numvertices, fp);
	
	for(int i = 0; i < p->numvertices; i++)
	{
		WriteVertex(p->vertices[i], fp);
	}
}
#endif


