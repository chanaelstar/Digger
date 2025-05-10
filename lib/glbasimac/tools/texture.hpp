/***************************************************************************
                         texture.hpp  -  description
                             -------------------
    copyright            : (C) 2012 by Biri
    email                : biri@univ-mlv.fr
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef _STP3D_TEXTURE_HPP_
#define _STP3D_TEXTURE_HPP_


#include <iostream>
#include <vector>
#include <cstring>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <setjmp.h>
#include "globals.hpp"

/** \addtogroup Macros */
/*@{*/
/** \defgroup type_texture Types de texture */
/*@{*/
/** Texture NULL. Utilisé à des fins de vérification uniquement. */
#define TEX_TYPE_NULL		99
/** Texture RVB. Texture classique (la plus utilisée). */
#define TEX_TYPE_RVB		0
/** Texture RVBA. Texture avec transparence. */
#define TEX_TYPE_RVBA		1
/** Texture LUM. Texture avec uniquement une luminance. */
#define TEX_TYPE_LUM		2
/*@}*/
/*@}*/

namespace STP3D {

	/**
	  * \brief Mesh class allows to store generic informations about a mesh 
	  * that has no indirect order.
	  * Mesh class allows to store several float buffer to use with a GL shaders.
	  * Such buffers are not interleaved and each has a semantic on his own. 
	  * Note that a mesh MUST have at least one buffer of coordinates.
	  * This class allows also the creation of the corresponding VBO.
	  * This class may or may not store the data.
	  * \TODO handle vertical flip for TGA reading
	  */
	class Texture2D {
	public:
		/// Standard construtor. Creates an empty texture withouh any information.
		Texture2D() : gl_id_tex(0),tex_w(0),tex_h(0),typetext(TEX_TYPE_NULL),last_tex_unit(GL_TEXTURE0),tabRVB(NULL) {};
		/// Constructor with file name
		/// \param fic file name
		Texture2D(const char* fic);
		/** Texture constructor with memory reservation (data = 0) or not.
		  * Texture constructor with memory reservation or not. If there is memory 
		  * reservation, data are 'null' (0) and the texture is black.
		  * Usefull for a delayed work. If no reservation, the data pointer is NULL,
		  * what is usefull for FBO for example.
		  * \param type Image type to be chosen in predefined texture type
		  * \param tex_win,tex_hin image size
		  * \param reserve Memory reservation or not (true by défaut)
		  * \see type_texture
		  */
		Texture2D(int type,size_t w,size_t h,bool reserve=true);
		~Texture2D() {if(tabRVB) delete[](tabRVB); tabRVB = NULL;};

		unsigned int gl_id_tex;		///< OpenGL id binding
		size_t tex_w;		///< Texture width (width)
		size_t tex_h;		///< Texture height (height)
		int typetext;		///< Texture type
		unsigned int last_tex_unit;	///< Last texture unit binding used

		unsigned char* getTab() const {return tabRVB;};

		/** OpenGL texture initialisation.
		  * This initialisation creates a OpenGL texture binding with the default parameter :
		  * - GL_TEXTURE_WRAP_R sets to GL_REPEAT
		  * - GL_TEXTURE_WRAP_S sets to GL_REPEAT
		  * - GL_TEXTURE_MAG_FILTER sets to GL_LINEAR
		  * - GL_TEXTURE_MIN_FILTER sets to GL_LINEAR
		  */
		void initTexture();
		/** Set texture filtering parameters.
		  * This function sets the two wrapping parameters of GL_TEXTURE_MAG_FILTER and GL_TEXTURE_MIN_FILTER
		  * to value this be choose in 
		  * - GL_NEAREST
		  * - GL_LINEAR
		  *
		  * \param param_min Min filtering : choice in  GL_LINEAR, GL_NEAREST, GL_NEAREST_MIPMAP_NEAREST,
		  *   GL_LINEAR_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR
		  * \param param_max Max filtering : choice in GL_LINEAR, GL_NEAREST
		  */
		void setTextureFiltering(GLenum param_min,GLenum param_max);
		/** Set texture wraping parameters.
		  * This function sets the two wrapping parameters GL_WRAP_S and GL_WRAP_T
		  * to values to be chosen in 
		  * - GL_CLAMP
		  * - GL_CLAMP_TO_EDGE
		  * - GL_REPEAT
		  *
		  * \param param_s s coordinate wraping
		  * \param param_t t coordinate wraping
		  */
		void setTextureWraping(GLenum param_s,GLenum param_t);
		/** OpenGL texture loading
		  * \param target_tex Binding point for the texture (like GL_TEXTURE0_ARB which is the default parameter)
		  */	
		void loadTexture(GLuint target_tex = GL_TEXTURE0);
		/** OpenGL Unloading of the texture.
		  * \param target_tex Binding point for the texture (like GL_TEXTURE0_ARB which is the default parameter)
		  */	
		void unloadTexture(GLuint target_tex = GL_TEXTURE0);
	private:
		bool loadImage(std::string* nmfile); // Chargement d'une image par nom de fichier
		bool readPPM(FILE*f);					// Lecture d'un PPM
		bool readJPG(std::string* nmfile);	// Lecture d'un JPG
		bool readRGBA(int sizex,int sizey,FILE* f);	// Lecture d'un RGBA
		bool readTGA(FILE* f);					// Lecture d'un TGA
		bool readTGANoCompress(FILE *file,int d);	// Lecture d'un TGA non compresse
		bool readTGACompress(FILE *file,int d);		// Lecture d'un TGA compresse
		void symVertical(int src_w,int src_h,int nbchannel,unsigned char *src);
		unsigned char* tabRVB; 		///< Donnees sources de l'image
	};

	typedef struct
	{
		unsigned char header[6];
		unsigned int bytesPerPixel;
		unsigned int imageSize;
		unsigned int temp;
		unsigned int type;
		unsigned int Height;
		unsigned int Width;
		unsigned int Bpp;
	} TGA;

	typedef struct
	{
		unsigned char Header[18];
	} TGAHeader;

	/* *************************************************************************************
	 * ********** CONSTRUCTEUR & DESTRUCTEURS
	 * ************************************************************************************* */
	inline void Texture2D::symVertical(int src_w,int src_h,int nbchannel,unsigned char *src) {
		unsigned char* tmp = new unsigned char[src_w*src_h*nbchannel];
		unsigned char* bas=src;
		unsigned char* haut=tmp+(src_h-1)*src_w*nbchannel;

		for(int i=0;i<src_h;i++) {
			// La ligne du bas va en tmp
			memcpy(haut,bas,src_w*nbchannel);
			bas+=src_w*nbchannel;
			haut-=src_w*nbchannel;
		}
		memcpy(src,tmp,nbchannel*src_w*src_h);
		delete[](tmp);
		return;
	}

	inline Texture2D::Texture2D(const char* fic) {
		std::string *nomfic = new std::string((char*)fic);
		last_tex_unit = GL_TEXTURE0;
		gl_id_tex = tex_w = tex_h = 0;
		tabRVB = NULL;
		typetext = TEX_TYPE_NULL;
		loadImage(nomfic);
		if (typetext == TEX_TYPE_LUM) symVertical(tex_w,tex_h,1,getTab());
		if (typetext == TEX_TYPE_RVB) symVertical(tex_w,tex_h,3,getTab());
		if (typetext == TEX_TYPE_RVBA) symVertical(tex_w,tex_h,4,getTab());
		delete(nomfic);
	}

	inline Texture2D::Texture2D(int type,size_t tex_win,size_t tex_hin,bool reserve) {
		int decal = -1;
		
		gl_id_tex = 0;
		last_tex_unit = GL_TEXTURE0;
		tex_w = tex_win;
		tex_h = tex_hin;
		typetext = type;
		switch (type) {
			case (TEX_TYPE_NULL) : 
				decal = 0;
				tabRVB = NULL;
				break;
			case (TEX_TYPE_LUM) :
				decal = 1;
				break;
			case (TEX_TYPE_RVB) :
				decal = 3;
				break;
			case (TEX_TYPE_RVBA) :
				decal = 4;
				break;
			default :
				STP3D::setError("[Texture : constructor] Unknown ("+intToString(type)+") texture type loading into texture constructor");
		}
		if (decal > 0 && reserve) {
			tabRVB = new unsigned char[tex_w*tex_h*decal];
			if (tabRVB == NULL) {
				STP3D::setError("[Texture : constructor] Unable to create data for texture loading. This texture have not been initialized");
			}
			else {
				memset(tabRVB,0,tex_w*tex_h*decal);
			}
		}
		else {
			tabRVB = NULL;
		}
	}


	/* *************************************************************************************
	 * ********** FONCTIONS D'INTERACTION AVEC OPENGL
	 * ************************************************************************************* */

	inline void Texture2D::initTexture() {
		glGenTextures(1,&gl_id_tex);
		glBindTexture(GL_TEXTURE_2D,gl_id_tex);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

		if (typetext==TEX_TYPE_RVB) {
			glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,tex_w,tex_h,0,GL_RGB,GL_UNSIGNED_BYTE,getTab());
		}
		else if (typetext==TEX_TYPE_RVBA) {
			glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,tex_w,tex_h,0,GL_RGBA,GL_UNSIGNED_BYTE,getTab());
		}
		else if (typetext==TEX_TYPE_LUM){
			glTexImage2D(GL_TEXTURE_2D,0,GL_RED,tex_w,tex_h, 0,GL_RED,GL_UNSIGNED_BYTE,getTab());
		}
		else {
			STP3D::setError("[Texture : initTexture] NULL initialization of texture is impossible");
		}
		glBindTexture(GL_TEXTURE_2D,0);
		//cout<<"Fin initialisation Texture : "<<*this<<std::endl;
	}

	inline void Texture2D::setTextureWraping(GLenum param_s,GLenum param_t) {
		glBindTexture(GL_TEXTURE_2D,gl_id_tex);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,param_s);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,param_t);
		glBindTexture(GL_TEXTURE_2D,0);
	}

	inline void Texture2D::setTextureFiltering(GLenum param_min,GLenum param_max) {
		glBindTexture(GL_TEXTURE_2D,gl_id_tex);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,param_max);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,param_min);
		glBindTexture(GL_TEXTURE_2D,0);
	}

	inline void Texture2D::loadTexture(GLuint target_tex) {
		last_tex_unit = target_tex;
		glActiveTextureARB(target_tex);
		glBindTexture(GL_TEXTURE_2D,gl_id_tex);
	}

	inline void Texture2D::unloadTexture(GLuint target_tex) {
		last_tex_unit = target_tex;
		glActiveTextureARB(last_tex_unit);
		glBindTexture(GL_TEXTURE_2D,0);
	}

	/* *************************************************************************************
	 * ********** FONCTIONS DE CHARGEMENT 
	 * ************************************************************************************* */

	inline bool Texture2D::loadImage(std::string* nmfile) {
		int posfin;
		FILE* ffic = fopen(nmfile->c_str(),"r");
		if (ffic==NULL) {
			STP3D::setError("[Texture : loadImage] Unable to read file :"+(*nmfile)+")");
			return false;
		}
		
		std::string *indictype = new std::string(*nmfile);
		posfin = indictype->find_last_of('.',indictype->size());
		*indictype = indictype->erase(0,posfin+1);
		bool success = false;
		if ((indictype->compare("ppm")==0) || (indictype->compare("PPM")==0) || (indictype->compare("pgm")==0) || (indictype->compare("PGM")==0) ) {
			typetext = TEX_TYPE_RVB;
			success = readPPM(ffic);
			fclose(ffic);
		}
		else if ((indictype->compare("tga")==0) || (indictype->compare("TGA")==0)) {
			success = readTGA(ffic);
			fclose(ffic);
		}
		/*
		else if ((indictype->compare("jpg")==0) || (indictype->compare("JPG")==0)) {
			typetext = TEX_TYPE_RVB;
			lectureJpg(nmfile);
		}
		*/
		else {
			STP3D::setError("[Texture : loadImage] Erreur file format. Handled format are : .ppm / .jpg / .tga ");
		}
		delete(indictype);
		return success;
	}

	inline bool Texture2D::readPPM(FILE *f) {
		int maxrvb;
		bool flagp5 = false;
		char tamp[50] = {'\0'};
		size_t t;

		//f.getline(tamp,50,'\n');
		t = fscanf(f,"%s",tamp);
		for (unsigned int i=0;i<50;i++) if (tamp[i]==' ') {tamp[i] = '\0';}

		if (strcmp(tamp,"P6") && strcmp(tamp,"P5")) {
			STP3D::setError("[Texture : readPPM] File seems not to be a .ppm. Header is "+std::string(tamp)+"("+intToString(t)+")");
			return false;
		}
		if (!strcmp(tamp,"P5")) flagp5 = true;
		//f.get(tamp,50,'\n');
		while(fgetc(f)!='\n');
		// Une ligne de commentaire...
		while(fgetc(f)!='\n');
		//f.get();
		// Recuperation dimension
		//f >> tex_w >>tex_h;
		//f >> maxrvb;
		t = fscanf(f,"%ld %ld",&tex_w,&tex_h);

		//f.get(tamp,50,'\n');
		//f.get();
		while(fgetc(f)!='\n');
		t = fscanf(f,"%d",&maxrvb);
		
		while(fgetc(f)!='\n');
		if (maxrvb != 255) {
			STP3D::setError("[Texture : readPPM] Color quantization is different from 255 : "+std::string(tamp));
			return false;
		}
		if (flagp5) {
			typetext = TEX_TYPE_LUM;
			tabRVB = new unsigned char[tex_w*tex_h];
			if (tabRVB == NULL) {
				STP3D::setError("[Texture : readPPM] Unable to allocate memory (memory full ?)");
				return false;
			}
			for(unsigned int i=0;i<tex_h;i++) {
				t = fread(tabRVB+(tex_h-i-1)*tex_w,sizeof(unsigned char),tex_w,f);
			}
		}
		else {
			tabRVB = new unsigned char[tex_w*tex_h*3];
			if (tabRVB == NULL) {
				STP3D::setError("[Texture : readPPM] Unable to allocate memory (memory full ?)");
				return false;
			}
			for(unsigned int i=0;i<tex_h;i++) {
				t = fread(tabRVB+(tex_h-i-1)*3*tex_w,sizeof(unsigned char),tex_w*3,f);
			}	
		}
		return true;
	}


	inline bool Texture2D::readTGA(FILE *file) {
		TGAHeader tgaheader;
		unsigned char uTGAcompare = 2;
		unsigned char cTGAcompare = 10;
		bool vertical_flip = false;
		bool horizont_flip = false;
		char descriptor;
		int d;

		if (tabRVB != NULL) {
			delete[](tabRVB);
			tabRVB = NULL;
		}
		if(fread(&tgaheader, sizeof(TGAHeader), 1, file) == 0) {
			STP3D::setError("[Texture : readTGA] Could not read file header");
	    	return false;
		}
		int xorigine = tgaheader.Header[9]*256 + tgaheader.Header[8];
		int yorigine = tgaheader.Header[11]*256 + tgaheader.Header[10];
		tex_w = *((unsigned short*) (tgaheader.Header+12));
		tex_h = *((unsigned short*) (tgaheader.Header+14));
		descriptor = tgaheader.Header[17];

		if (descriptor && 0x20) vertical_flip = true;
		if (descriptor && 0x10) horizont_flip = true;
		
		if (horizont_flip) {
			STP3D::setError("[Texture : readTGA] horizontal mirroring not handled ");
		}

		//std::cerr<<"Taille : "<<tex_w<<" / "<<tex_h<<std::endl;
		d = tgaheader.Header[16];
		//std::cerr<<"Origine "<<xorigine<<" / "<<yorigine<<std::endl;
		if ((xorigine != 0) || (yorigine != 0)) {
			STP3D::setError("[Texture : readTGA] No handle of offset in TGA ");
			return false;
		}
		bool success = false;
		if (tgaheader.Header[2] == uTGAcompare) {
			success = readTGANoCompress(file,d);
		}
		else if (tgaheader.Header[2] == cTGAcompare) {
			success = readTGACompress(file,d);
		}
		else {
			STP3D::setError("[Texture : readTGA] TGA file should be type 2 (colour no compress) or type 10 (colour compress)");
			return false;
		}

		if (vertical_flip) {
			STP3D::setError("[Texture : readTGA] We do not handle vertical flip. But the texture is still loaded");
			//CompImage::symVertical(tex_w,tex_h,tabRVB);
		}
		return success;
	}

	inline bool Texture2D::readTGANoCompress(FILE *file,int d) {
		unsigned int iswap;	
		unsigned char cswap;	
		int nbpixel = 3;

		if((tex_w <= 0) || (tex_h <= 0) || ((d != 24) && (d !=32)))	{
			STP3D::setError("[Texture : readTGA] Error invalid texture information d="+intToString(d)+" tex_w="+intToString(tex_w)+" tex_h="+intToString(tex_h));
			return false;
		}

		if(d == 32) {
			typetext = TEX_TYPE_RVBA;
			nbpixel = 4;
		}
		else {
			typetext = TEX_TYPE_RVB;
		}

		unsigned int size = nbpixel * tex_w * tex_h;
		tabRVB	= new unsigned char[size];

		if(tabRVB == NULL) {
			STP3D::setError("[Texture : readTGANoCompress] Unable to allocate memory (memory full ?)");
			return false;
		}

		if(fread(tabRVB, 1, size, file) != size) {
			STP3D::setError("[Texture : readTGANoCompress] Could not read image data");
			if (tabRVB != NULL) delete[](tabRVB);
			tabRVB = NULL;
			return false;
		}

		for(iswap = 0; iswap < size; iswap += nbpixel) {
			cswap = tabRVB[iswap+2];
			tabRVB[iswap+2] = tabRVB[iswap];
			tabRVB[iswap] = cswap;
		}
		//tabRVB[iswap] ^= tabRVB[iswap+2] ^= tabRVB[iswap] ^= tabRVB[iswap+2];

		return true;
	}

	inline bool Texture2D::readTGACompress(FILE *file,int d) {
		unsigned int pixelcount;
		unsigned int currentpixel	= 0;
		unsigned int currentbyte	= 0;
		unsigned char* colorbuffer;
		int size;
		int bpp;


		pixelcount = tex_w * tex_h;

		if((tex_w <= 0) || (tex_h <= 0) || ((d != 24) && (d !=32)))	{
			STP3D::setError("[Texture : readTGACompress] Error invalid texture information d="+intToString(d)+" tex_w="+intToString(tex_w)+" tex_h="+intToString(tex_h));
			return false;
		}

		bpp	= (d / 8);
		size = (bpp * tex_w * tex_h);
		tabRVB = new unsigned char[size];
		colorbuffer = (unsigned char *)malloc(bpp);

		if(bpp == 4) {
			//std::cerr<<"Type = TYPE RVBA"<<std::endl;
			typetext = TEX_TYPE_RVBA;
		}
		else {
			//std::cerr<<"Type = TYPE RVB"<<std::endl;
			typetext = TEX_TYPE_RVB;
		}

		if(tabRVB == NULL) {
			STP3D::setError("[Texture : readTGACompress] Unable to allocate memory (memory full ?)");
			return false;
		}

		do {
			unsigned char chunkheader = 0;
			if(fread(&chunkheader, sizeof(unsigned char), 1, file) == 0) {
				STP3D::setError("[Texture : readTGACompress] Error could not read RLE header");
				if(tabRVB != NULL) delete[](tabRVB);
				tabRVB = NULL;
				return false;
			}

			if(chunkheader < 128) {
				short counter;
				chunkheader++;		
				for(counter = 0; counter < chunkheader; counter++) {
					if(fread(colorbuffer, 1, bpp, file) != (unsigned int)bpp ) {
						STP3D::setError("[Texture : readTGACompress] Error could not read image data");
						if(colorbuffer != NULL) free(colorbuffer);
						if(tabRVB != NULL) delete[](tabRVB);
						tabRVB = NULL;
						return false;
		      		}
					/* write to memory */
					tabRVB[currentbyte		] = colorbuffer[2];
					tabRVB[currentbyte + 1	] = colorbuffer[1];
					tabRVB[currentbyte + 2	] = colorbuffer[0];

					if(bpp == 4)			
					{
						tabRVB[currentbyte + 3] = colorbuffer[3];
					}

					currentbyte += bpp;
					currentpixel++;	

					if(currentpixel > pixelcount) {
						STP3D::setError("[Texture : readTGACompress] Error too many pixels read");
						if(colorbuffer != NULL) free(colorbuffer);
						if(tabRVB != NULL) delete[](tabRVB);
						tabRVB = NULL;
						return false;
					}
		    	}
			}
			else {
				short counter;
				chunkheader -= 127;						      
				if(fread(colorbuffer, 1, bpp, file) != (unsigned int)bpp){
					STP3D::setError("[Texture : readTGACompress] Error could not read image data");
					if(colorbuffer != NULL) free(colorbuffer);
					if(tabRVB != NULL) delete[](tabRVB);
					tabRVB = NULL;
					return false;
	    		}
		  
				for(counter = 0; counter < chunkheader; counter++) {
					tabRVB[currentbyte		] = colorbuffer[2];
					tabRVB[currentbyte + 1	] = colorbuffer[1];
					tabRVB[currentbyte + 2	] = colorbuffer[0];
					if(bpp == 4) tabRVB[currentbyte + 3] = colorbuffer[3];

					currentbyte += bpp;
					currentpixel++;
					if(currentpixel > pixelcount){
						STP3D::setError("[Texture : readTGACompress] Error too many pixels read");
						if(colorbuffer != NULL) free(colorbuffer);
						if(tabRVB != NULL) delete[](tabRVB);
						tabRVB = NULL;
						return false;
					}
				}
			}
		} while(currentpixel < pixelcount);

		return true;
	}


	// Ecriture d'une texture sur flot d'ecriture
	inline std::ostream& operator<<(std::ostream& os, const Texture2D& src) {
		os << "Texture (type "<<src.typetext<<") indbind "<<src.gl_id_tex<<" size "<<src.tex_w<<"x"<<src.tex_h<<std::endl;
		return os;
	}

};

#endif


