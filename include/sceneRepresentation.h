/*****************************************************************************
**      Stereo VO and SLAM by combining point and line segment features     **
******************************************************************************
**                                                                          **
**  Copyright(c) 2016-2018, Ruben Gomez-Ojeda, University of Malaga         **
**  Copyright(c) 2016-2018, David Zuñiga-Noël, University of Malaga         **
**  Copyright(c) 2016-2018, MAPIR group, University of Malaga               **
**                                                                          **
**  This program is free software: you can redistribute it and/or modify    **
**  it under the terms of the GNU General Public License (version 3) as     **
**  published by the Free Software Foundation.                              **
**                                                                          **
**  This program is distributed in the hope that it will be useful, but     **
**  WITHOUT ANY WARRANTY; without even the implied warranty of              **
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the            **
**  GNU General Public License for more details.                            **
**                                                                          **
**  You should have received a copy of the GNU General Public License       **
**  along with this program.  If not, see <http://www.gnu.org/licenses/>.   **
**                                                                          **
*****************************************************************************/

#include <iomanip>
#include <opencv2/core/types.hpp>
using namespace std;

#include <boost/shared_ptr.hpp>

#include <mrpt/opengl.h>
#include <mrpt/gui.h>
#include <mrpt/config/CConfigFile.h>
#include <mrpt/config/CConfigFileBase.h>
#include <mrpt/maps/CPointsMap.h>
#include <mrpt/maps/CColouredPointsMap.h>
#include <mrpt/maps/CSimplePointsMap.h>
#include <mrpt/img/CImage.h>
#include <list>
using namespace mrpt;
using namespace mrpt::gui;
using namespace mrpt::poses;
using namespace mrpt::config;
using namespace mrpt::math;
using namespace mrpt::opengl;
using namespace mrpt::maps;
using mrpt::img::CImage;

//#include <opencv/cv.h>
using namespace cv;

#include <eigen3/Eigen/Core>
using namespace Eigen;

#include <stereoFeatures.h>
using namespace StVO;

class sceneRepresentation{

public:

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW;

    sceneRepresentation();
    sceneRepresentation(string configFile);
    ~sceneRepresentation();
    void initialize3DScene(Matrix4d x_0);
//    void initialize3DSceneLines(Matrix4d x_0);
//    void initialize3DSceneGT(Matrix4d x_0);

    void initializeScene(Matrix4d x_0, bool has_gt);
//    void initializeScene(Matrix4d x_0, Matrix4d x_0gt);

    bool updateScene(); 
    bool updateScene(std::list<PointFeature *> matched_pt, list<LineFeature *> matched_ls);
    void plotPointsCovariances();
    void plotLinesCovariances();

    void setText(int frame_, float time_, int nPoints_, int nPointsH_, int nLines_, int nLinesH_);
    void setCov(MatrixXd cov_);
    void setPose(Matrix4d x_);
    void setGT(Matrix4d xgt_);
    void setComparison(Matrix4d xcomp_);
    void setImage(const Mat &image_);
    void setImage(const string &image_);
    void setLegend();
    void setHelp();
    void setPoints(CMatrixFloat pData_);
    void setLines(CMatrixFloat lData_);
    void setStereoCalibration(Matrix3d K_, float b_);
    void setKF();
    void setKF(Matrix4d Tfw);

    bool waitUntilClose();
    bool isOpen();
    bool getYPR(float &yaw, float &pitch, float &roll);
    bool getPose(Matrix4d &T);

private:

    CMatrixDouble getPoseFormat(Matrix4d T);
    CMatrixDouble33 getCovFormat(MatrixXd cov_);
    CPose3D getPoseXYZ(VectorXd x);

    CDisplayWindow3D*           win;
    COpenGLScene::Ptr             theScene;
    COpenGLViewport::Ptr          image, legend, help;
    opengl::CSetOfObjects::Ptr    bbObj, bbObj1, srefObj, srefObj1, gtObj, srefObjGT, elliObjL, elliObjP;
    opengl::CEllipsoid3D::Ptr       elliObj;
    opengl::CSetOfLines::Ptr      lineObj;
    opengl::CPointCloud::Ptr      pointObj;

    //CPointsMapPtr pointsObj;

    opengl::CFrustum::Ptr         frustObj, frustObj1;
    opengl::CAxis::Ptr            axesObj;


    float           sbb, saxis, srad, sref, sline, sfreq, szoom, selli, selev, sazim, sfrust, slinef;
    CVectorFixedDouble<6>  v_aux, v_aux_, v_aux1, v_aux1_, v_auxgt, gt_aux_, v_auxgt_;
    CPose3D         pose, pose_0, pose_gt, pose_ini, ellPose, pose1,  change, frustumL_, frustumR_;
    Matrix4d        x_ini;
    mrptKeyModifier kmods;
    int             key;
    CMatrixDouble33 cov3D;
    bool            hasCamFix, hasText, hasCov, hasGT, hasChange, hasImg, hasLines, hasPoints, hasFrustum, hasComparison, hasLegend, hasHelp, hasAxes, hasTraj, isKitti;

    Matrix4d        x, xgt, xcomp;
    MatrixXd        cov, W;
    unsigned int    frame, nPoints, nPointsH, nLines, nLinesH;
    float           time;
    string          img, img_legend, img_help;
    CMatrixFloat    lData, pData;
    CImage          img_mrpt_legend, img_mrpt_image, img_mrpt_help;

    float           b, sigmaP, sigmaL, f, cx, cy, bsigmaL, bsigmaP;

    cv::Size img_sz;

};

