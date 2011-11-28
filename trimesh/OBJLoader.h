#ifndef OBJLOADER_H
#define	OBJLOADER_H


#include <stdio.h>
#include <string.h>

#define Debug(x){MessageBox(NULL, x, "Debug:", MB_OK);}

struct vertex
{
	float x, y, z;
	unsigned int index;	
};
typedef vertex normal;
struct texcoord
{
	float u, v;
	unsigned int index;
};

struct face
{
	float x[3], y[3], z[3];		//vertices
	float u[3], v[3];		//text coords
	float a[3], b[3], c[3];		//normals
};

struct material
{	
	char name[32];
	int illum;
	char map_Kd[32];
	float Ni;
	float Kd[3];
	float Ka[3];
	float Tf[3];
	unsigned int *texture;
};

class model
{
private:
	int faces;
	int coords;
	int normals;
	bool loaded;
	char mtllib[256];
	char filename[256];
	char directory[256];
	unsigned int vindex;
	unsigned int tindex;
	unsigned int nindex;
	struct vnode
	{
		vertex data;
		vnode * next;
	};
	struct tnode
	{
		texcoord data;
		tnode * next;
	};
	
	struct mnode
	{
		material data;
		mnode * next;
	};
	struct fnode
	{
		face data;
		mnode * mat;
		fnode * next;
	};

	vnode * vfirst;
	vnode * vcurrent;
	tnode * tfirst;
	tnode * tcurrent;
	vnode * nfirst;
	vnode * ncurrent;
	fnode * ffirst;
	fnode * fcurrent;
	mnode * mfirst;
	mnode * mcurrent;
	void skipComment(FILE *);
	
	bool loadVertex(FILE *);
	bool loadTexCoord(FILE *);
	bool loadNormal(FILE *);
	bool loadFace(FILE *);
	bool loadMaterialLib(FILE *);
	void loadMaterials(FILE *);
	void useMaterial(FILE *);
	
public:
	bool Load(char * objfile);
	model()
	{
		vfirst=vcurrent= NULL;
		tfirst=tcurrent= NULL;
		nfirst=ncurrent= NULL;
		ffirst=fcurrent= NULL;
		mfirst=mcurrent= NULL;
		vindex=tindex=nindex= 1;
	}
	void draw();
	float Y(float, float){return 0.0;}
	bool Collide(float, float, float,float, float, float){return false;}
};

bool model::Load(char * objfile)
{
	char buffer[256];
	strcpy(filename, objfile);
	FILE * file = fopen(filename, "r");

	if(file == NULL)
	{
		MessageBox(NULL, objfile, "Model file not found:", MB_OK);
		return false;
	}
	while(fscanf(file, "%s", buffer) != EOF)
	{
		if(!strcmp("#", buffer))skipComment(file);
		if(!strcmp("mtllib", buffer))loadMaterialLib(file);
		if(!strcmp("v", buffer))loadVertex(file);
		if(!strcmp("vt", buffer))loadTexCoord(file);
		if(!strcmp("vn", buffer))loadNormal(file);
		if(!strcmp("f", buffer))loadFace(file);
		if(!strcmp("s", buffer))fscanf(file, "%s", buffer); 
		if(!strcmp("usemtl", buffer))useMaterial(file);
		
	}
	fclose(file);
	loaded = true;
	return true;
}

void model::useMaterial(FILE * file)
{
	char buffer[256];
	mnode * cursor = mfirst;
	fscanf(file, "%s", buffer);
	while(strcmp(buffer, cursor->data.name))
		cursor= cursor->next;
	mcurrent = cursor;
}

void model::skipComment(FILE * file)
{
	char buffer[256];
	fgets(buffer, 256, file);
}

bool model::loadMaterialLib(FILE * file)
{
	char * wd = strtok(filename, "/");
	char buffer[256];
	fscanf(file, "%s", buffer);
	sprintf(mtllib, "%s/%s", wd, buffer);
	strcpy(directory, wd);
	FILE * lib = fopen(mtllib, "r");
	if(lib == NULL)
	{
		MessageBox(NULL, mtllib, "Material library not found:", MB_OK);
		return false;
	}
	else loadMaterials(lib);
	fclose(lib);
	return true;
}

void model::loadMaterials(FILE * file)
{
	
	char parameter[32];
	mnode * temp;
	while(fscanf(file, "%s", parameter) != EOF)
	{
		
		if(!strcmp("newmtl", parameter))
		{
			temp = new mnode();
			fscanf(file, "%s", temp->data.name);
		}
		if(!strcmp("illum", parameter))
			fscanf(file, "%i", &temp->data.illum);
		if(!strcmp("map_Kd", parameter))
		{
			fscanf(file, "%s", temp->data.map_Kd);
			if(strstr(temp->data.map_Kd, ".bmp") != NULL)
			{
				char buffer[256];
				sprintf(buffer, "%s/%s", directory, temp->data.map_Kd);
				//#ifdef __GL_H__
				//temp->data.texture = new unsigned int();
				//loadTexture(buffer, temp->data.texture, 1);
				//#endif
			}
		}
		if(!strcmp("Ni", parameter))
			fscanf(file, "%f", &temp->data.Ni);
		if(!strcmp("Kd", parameter))
			fscanf(file, "%f %f %f", &temp->data.Kd[0],&temp->data.Kd[1],&temp->data.Kd[2]);
		if(!strcmp("Ka", parameter))
			fscanf(file, "%f %f %f", &temp->data.Ka[0],&temp->data.Ka[1],&temp->data.Ka[2]);
		if(!strcmp("Tf", parameter))
		{
			fscanf(file, "%f %f %f", &temp->data.Tf[0],&temp->data.Tf[1],&temp->data.Tf[2]);
			if(mfirst == NULL)
			{
				mfirst = temp;
				mcurrent = temp;
				mfirst->next = NULL;
			}
			else
			{
				mcurrent->next = temp;
				mcurrent = mcurrent->next;
				mcurrent->next = NULL;
			}
			
		}
	}
		
}



bool model::loadVertex(FILE * file)
{
	vnode * temp = new vnode();
	fscanf(file, "%f %f %f", &temp->data.x, &temp->data.y, &temp->data.z);
	temp->data.index = vindex;
	if(vfirst == NULL)
	{
		vfirst = temp;
		vcurrent = temp;
		vfirst->next = NULL;
	}
	else
	{
		vcurrent->next = temp;
		vcurrent = vcurrent->next;
		vcurrent->next = NULL;
	}
	vindex++;
	return true;
}

bool model::loadTexCoord(FILE * file)
{
	tnode * temp = new tnode();
	fscanf(file, "%f %f", &temp->data.u, &temp->data.v);
	temp->data.index = tindex;
	temp->next = NULL;
	if(tfirst == NULL)
	{
		tfirst = temp;
		tcurrent = temp;
	}
	else
	{
		tcurrent->next = temp;
		tcurrent = tcurrent->next;
	}
	tindex++;
	return true;
}

bool model::loadNormal(FILE * file)
{
	vnode * temp = new vnode();
	fscanf(file, "%f %f %f", &temp->data.x, &temp->data.y, &temp->data.z);
	temp->data.index = nindex;
	temp->next = NULL;
	if(nfirst == NULL)
	{
		nfirst = temp;
		ncurrent = temp;
	}
	else
	{
		ncurrent->next = temp;
		ncurrent = ncurrent->next;
	}
	nindex++;
	return true;
}

bool model::loadFace(FILE * file)
{
	fnode * temp = new fnode();
	temp->mat = mcurrent;
	vnode * vcursor = vfirst;
	tnode * tcursor = tfirst;
	vnode * ncursor = nfirst;
	unsigned int v_index[3], t_index[3], n_index[3];
	for(int i = 0; i < 3; i++)
	{
		vcursor = vfirst;
		tcursor = tfirst;
		ncursor = nfirst;
		fscanf(file, "%i/%i/%i", &v_index[i], &t_index[i], &n_index[i]);
		
		for(int v = 1; v != v_index[i]; v++)
			vcursor = vcursor->next;
		temp->data.x[i] = vcursor->data.x;
		temp->data.y[i] = vcursor->data.y;
		temp->data.z[i] = vcursor->data.z;

		for(int k = 1; k != t_index[i]; k++)
			tcursor = tcursor->next;
		temp->data.u[i] = tcursor->data.u;
		temp->data.v[i] = tcursor->data.v;
		
		for(int j = 1; j != n_index[i]; j++)
			ncursor = ncursor->next;
		temp->data.a[i] = ncursor->data.x;
		temp->data.b[i] = ncursor->data.y;
		temp->data.c[i] = ncursor->data.z;
	}
	
	temp->next = NULL;
	if(ffirst == NULL)
	{
		ffirst = temp;
		fcurrent = temp;
		ffirst->next = NULL;
	}
	else
	{
		fcurrent->next = temp;
		fcurrent = fcurrent->next;
		fcurrent->next = NULL;
	}
	faces++;
	return true;
}

void model::draw()
{
#ifdef __GL_H__
	if(loaded)
	{
		fnode * fcursor = ffirst;
		glBindTexture(GL_TEXTURE_2D, *ffirst->mat->data.texture);
		glBegin(GL_TRIANGLES);
		while(fcursor != NULL)
		{
			glColor3f(1.0f, 1.0f, 1.0f);
			
			glTexCoord2f(fcursor->data.u[0], fcursor->data.v[0]);
			glNormal3f(fcursor->data.a[0], fcursor->data.b[0], fcursor->data.c[0]);
			glVertex3f(fcursor->data.x[0], fcursor->data.y[0], fcursor->data.z[0]);

			glTexCoord2f(fcursor->data.u[1], fcursor->data.v[1]);
			glNormal3f(fcursor->data.a[1], fcursor->data.b[1], fcursor->data.c[1]);
			glVertex3f(fcursor->data.x[1], fcursor->data.y[1], fcursor->data.z[1]);

			glTexCoord2f(fcursor->data.u[2], fcursor->data.v[2]);
			glNormal3f(fcursor->data.a[2], fcursor->data.b[2], fcursor->data.c[2]);
			glVertex3f(fcursor->data.x[2], fcursor->data.y[2], fcursor->data.z[2]);
			fcursor = fcursor->next;
		}
		glEnd();
	}
#endif
}

#endif	/* OBJLOADER_H */
