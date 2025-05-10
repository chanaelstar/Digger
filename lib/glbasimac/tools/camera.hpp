/***************************************************************************
                          camera.hpp  -  description
                             -------------------
    copyright            : (C) 2012 by Venceslas Biri
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

#ifndef _STP3D_FPS_CAMERA_HPP_
#define _STP3D_FPS_CAMERA_HPP_


/// \TODO Create flag to define orthographic camera

#include <iostream>
#include "vector4d.hpp"
#include "vector3d.hpp"
#include "matrix4d.hpp"

#define STP3D_DEFAULT_LEFT_RIGHT 0.1*0.577350269
#define STP3D_DEFAULT_TOP_BOTTOM 0.1*0.577350269
#define STP3D_DEFAULT_NEAR       0.1
#define STP3D_DEFAULT_FAR        100.0


namespace STP3D {

/** The camera class allows to handle a FPS like camera.
  * The camera class is designed to allow the movement of a camera along three axes related to a point of view. 
  * This class controls also the projection
  * @author Venceslas Biri
  */
class FPSCamera {
public:
	/***************************************************************************
	 *                              CONSTRUCTORS
	 ***************************************************************************/
	FPSCamera() {
		position_cam = Vector3D();
		view_dir_cam = dir_init_cam = Vector3D(0.0,0.0,-1.0);
		top__dir_cam = top_init_cam = Vector3D(0.0,1.0,0.0);
		orientat_cam = Vector3D();
		reactuOrientation();
		projMatrix = Matrix4D::perspective(-1.0*STP3D_DEFAULT_LEFT_RIGHT,STP3D_DEFAULT_LEFT_RIGHT,
		                                    -1.0*STP3D_DEFAULT_TOP_BOTTOM,STP3D_DEFAULT_TOP_BOTTOM,
		                                    STP3D_DEFAULT_NEAR,STP3D_DEFAULT_FAR);
	};
	FPSCamera(Vector3D pos_init, Vector3D dir_init,
	          Vector3D top_init = Vector3D(0.0,1.0,0.0),
	          Vector3D orient_init = Vector3D()) {
		position_cam = pos_init;
		view_dir_cam = dir_init_cam = dir_init;
		top__dir_cam = top_init_cam = top_init;
		orientat_cam = orient_init;
		reactuOrientation();
		projMatrix = Matrix4D::perspective(-1.0*STP3D_DEFAULT_LEFT_RIGHT,STP3D_DEFAULT_LEFT_RIGHT,
		                                    -1.0*STP3D_DEFAULT_TOP_BOTTOM,STP3D_DEFAULT_TOP_BOTTOM,
		                                    STP3D_DEFAULT_NEAR,STP3D_DEFAULT_FAR);
	};
	~FPSCamera() {};

	/// Intrisic parameters (for projections)
	Matrix4D projMatrix;
	Matrix4D viewMatrix;

	/** \name Projection related functions
	  */
	//@{
	/// Return the perpective projection matrix (use with GL)
	Matrix4D returnProjection() {return projMatrix;};
	/// Get the perpective projection matrix (use with GL)
	float* getProjection() {return (float*)projMatrix;};
	/// Set the projection for the camera
	void setIntrisic(Matrix4D newProj) {projMatrix = newProj;};
	/// Set the projection for the camera
	void setProjection(Matrix4D newProj) {projMatrix = newProj;};
	//@}

	/// Return the camera to its initial position and orientation
	void reinitPosCam(Vector3D pos_init,Vector3D dir_init,
	                  Vector3D top_init = Vector3D(0.0,1.0,0.0),
	                  Vector3D orient_init = Vector3D());

	/// Deprecated in OpenGL 3+
	float* getViewMatrix() {
		return (float*)viewMatrix;
	};
	Matrix4D returnViewMatrix() {
		return viewMatrix;
	};

	/** \name Fonctions de déplacement générique (déplacement / orientation)
	  * \todo English traduction
	  */
	//@{
	/// \brief Move front
	/// Moving toward view vector
	/// \param step moving step
	void stepFront(float step) {position_cam += view_dir_cam*step;reactuViewMatrix();};
	/// Déplacement en arrière
	/// Déplacement à l'opposé du vecteur de vue
	/// \param step pas du déplacement
	void stepBack(float step) {position_cam -= view_dir_cam*step;reactuViewMatrix();};
	/// Déplacement à gauche
	/// Déplacement dans le sens du produit vectoriel vecteur haut*vecteur de vue
	/// \param step pas du déplacement
	void stepLeft(float step) {Vector3D left_vec = top__dir_cam^view_dir_cam;position_cam += left_vec*step;reactuViewMatrix();};
	/// Déplacement à droite
	/// Déplacement dans le sens du produit vectoriel vecteur de vue*vecteur haut
	/// \param step pas du déplacement
	void stepRight(float step) {Vector3D left_vec = top__dir_cam^view_dir_cam;position_cam -= left_vec*step;reactuViewMatrix();};
	/// Déplacement en haut
	/// Déplacement dans le sens du vecteur haut
	/// \param step pas du déplacement
	void stepUp(float step) {position_cam += top__dir_cam*step;reactuViewMatrix();};
	/// Déplacement en bas
	/// Déplacement dans le sens opposé au vecteur haut
	/// \param step pas du déplacement
	void stepDown(float step) {position_cam -= top__dir_cam*step;reactuViewMatrix();};
	/// Regarde un cran à droite
	/// Rotation dans le sens horaire par rapport au vecteur haut
	/// \param angle angle de la rotation <b>en degré</b>
	void lookRight(float angle) {orientat_cam[1] -= angle*M_PI/180.0;reactuOrientation();};
	/// Regarde un cran à gauche
	/// Rotation dans le sens trigonométrique par rapport au vecteur haut
	/// \param angle angle de la rotation <b>en degré</b>
	void lookLeft(float angle) {orientat_cam[1] += angle*M_PI/180.0;reactuOrientation();};
	/// Regarde en haut
	/// Rotation dans le sens horaire par rapport au vecteur haut^vecteur vue
	/// \param angle angle de la rotation <b>en degré</b>
	void lookUp(float angle) {orientat_cam[0] -= angle*M_PI/180.0;reactuOrientation();};
	/// Regarde en bas
	/// Rotation dans le sens trigonométrique par rapport au vecteur haut^vecteur vue
	/// \param angle angle de la rotation <b>en degré</b>
	void lookDown(float angle) {orientat_cam[0] += angle*M_PI/180.0;reactuOrientation();};

	/// Position of the camera (related to the canonical GL frame)
	Vector3D position_cam;
	/// View vector of the camera (related to the canonical GL frame)
	Vector3D view_dir_cam;
	/// Initial view vector of the camera (related to the canonical GL frame)
	Vector3D dir_init_cam;
	/// Head/top vector of the camera (related to the canonical GL frame)
	Vector3D top__dir_cam;
	/// Initial head/top vector of the camera  (related to the canonical GL frame)
	Vector3D top_init_cam;
	/// Store the orientation of the camera along axes x,y and z
	/// \TODO translate to quaternion...
	Vector3D orientat_cam;

private:

	/// Setting view camera...
	void reactuViewMatrix();
	/// Reactualization of vectors after a rotation
	void reactuOrientation();
};


/***************************************************************************
 *                      OPERATION GL PROJ / TRANSFO
 ***************************************************************************/

inline void FPSCamera::reinitPosCam(Vector3D pos_init,Vector3D dir_init,
                             Vector3D top_init,
                             Vector3D orient_init){
	position_cam = pos_init;
	view_dir_cam = dir_init_cam = dir_init;
	top__dir_cam = top_init_cam = top_init;
	orientat_cam = orient_init;
	reactuOrientation();
}

/***************************************************************************
 *                          MOVEMENTS
 ***************************************************************************/

inline void FPSCamera::reactuViewMatrix() {
	viewMatrix = Matrix4D::lookAt(position_cam,position_cam+view_dir_cam,top__dir_cam);
}

inline void FPSCamera::reactuOrientation() {
	Matrix4D matt = Matrix4D::rotation(orientat_cam[1],1);
	Vector3D left_vec = (matt.xDir(top_init_cam^dir_init_cam)).homogeneate();
	Matrix4D matu = Matrix4D::rotation(orientat_cam[0],left_vec);
	top__dir_cam = (matu.xDir(top_init_cam)).homogeneate();
	view_dir_cam = (matu*(matt.xDir(dir_init_cam))).homogeneate();
	reactuViewMatrix();
}

/// Affichage de la camera sur un flux
inline std::ostream& operator<<(std::ostream& os,const FPSCamera& src) {
	os <<"\n**************************************"<<std::endl;
	os <<"           INFORMATIONS CAMERA          "<<std::endl;
	os <<" Position camera : "<<src.position_cam<<std::endl;
	os <<"\tview dir cam   : "<<src.view_dir_cam<<std::endl;
	os <<"\ttop dir cam    : "<<src.top__dir_cam<<std::endl;
	os <<"\tRot % x        : "<<src.orientat_cam[0]<<std::endl;
	os <<"\tRot % y        : "<<src.orientat_cam[1]<<std::endl;
	os <<"**************************************"<<std::endl;
	return os;
}

} // Namespace ends
#endif

