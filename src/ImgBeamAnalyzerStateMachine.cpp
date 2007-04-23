static const char *RcsId = "$Header: /users/chaize/newsvn/cvsroot/Calculation/ImgBeamAnalyzer/src/ImgBeamAnalyzerStateMachine.cpp,v 1.6 2007-04-23 13:58:26 julien_malik Exp $";
//+=============================================================================
//
// file :         ImgBeamAnalyzerStateMachine.cpp
//
// description :  C++ source for the ImgBeamAnalyzer and its alowed. 
//                method for commands and attributes
//
// project :      TANGO Device Server
//
// $Author: julien_malik $
//
// $Revision: 1.6 $
//
// $Log: not supported by cvs2svn $
//
// copyleft :     European Synchrotron Radiation Facility
//                BP 220, Grenoble 38043
//                FRANCE
//
//-=============================================================================
//
//  		This file is generated by POGO
//	(Program Obviously used to Generate tango Object)
//
//         (c) - Software Engineering Group - ESRF
//=============================================================================

#include <ImgBeamAnalyzer.h>
#include <ImgBeamAnalyzerClass.h>

/*====================================================================
 *	This file contains the methods to allow commands and attributes
 * read or write execution.
 *
 * If you wand to add your own code, add it between 
 * the "End/Re-Start of Generated Code" comments.
 *
 * If you want, you can also add your own methods.
 *====================================================================
 */

namespace ImgBeamAnalyzer_ns
{

//=================================================
//		Attributes Allowed Methods
//=================================================

//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_EnableImageStats_allowed
// 
// description : 	Read/Write allowed for EnableImageStats attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_EnableImageStats_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_EnableProfiles_allowed
// 
// description : 	Read/Write allowed for EnableProfiles attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_EnableProfiles_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_InputImage_allowed
// 
// description : 	Read/Write allowed for InputImage attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_InputImage_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_MeanIntensity_allowed
// 
// description : 	Read/Write allowed for MeanIntensity attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_MeanIntensity_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_XProfile_allowed
// 
// description : 	Read/Write allowed for XProfile attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_XProfile_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_YProfile_allowed
// 
// description : 	Read/Write allowed for YProfile attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_YProfile_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_ComputationPeriod_allowed
// 
// description : 	Read/Write allowed for ComputationPeriod attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_ComputationPeriod_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_EstimComputTime_allowed
// 
// description : 	Read/Write allowed for EstimComputTime attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_EstimComputTime_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_Enable2DGaussianFit_allowed
// 
// description : 	Read/Write allowed for Enable2DGaussianFit attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_Enable2DGaussianFit_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_GaussianFitMagnitude_allowed
// 
// description : 	Read/Write allowed for GaussianFitMagnitude attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_GaussianFitMagnitude_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_GaussianFitTilt_allowed
// 
// description : 	Read/Write allowed for GaussianFitTilt attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_GaussianFitTilt_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}

//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_XProfileSigma_allowed
// 
// description : 	Read/Write allowed for XProfileSigma attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_XProfileSigma_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_YProfileSigma_allowed
// 
// description : 	Read/Write allowed for YProfileSigma attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_YProfileSigma_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}

//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_GaussianFitChi2_allowed
// 
// description : 	Read/Write allowed for GaussianFitChi2 attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_GaussianFitChi2_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_GaussianFitParameterCovariance_allowed
// 
// description : 	Read/Write allowed for GaussianFitParameterCovariance attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_GaussianFitParameterCovariance_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_XProfileChi2_allowed
// 
// description : 	Read/Write allowed for XProfileChi2 attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_XProfileChi2_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_YProfileChi2_allowed
// 
// description : 	Read/Write allowed for YProfileChi2 attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_YProfileChi2_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_AlarmZone_allowed
// 
// description : 	Read/Write allowed for AlarmZone attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_AlarmZone_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_Fit2DNbIterMax_allowed
// 
// description : 	Read/Write allowed for Fit2DNbIterMax attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_Fit2DNbIterMax_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_Fit2DMaxRelChange_allowed
// 
// description : 	Read/Write allowed for Fit2DMaxRelChange attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_Fit2DMaxRelChange_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_Fit1DNbIterMax_allowed
// 
// description : 	Read/Write allowed for Fit1DNbIterMax attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_Fit1DNbIterMax_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_Fit1DMaxRelChange_allowed
// 
// description : 	Read/Write allowed for Fit1DMaxRelChange attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_Fit1DMaxRelChange_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_XProfileBG_allowed
// 
// description : 	Read/Write allowed for XProfileBG attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_XProfileBG_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_YProfileBG_allowed
// 
// description : 	Read/Write allowed for YProfileBG attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_YProfileBG_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_GaussianFitBG_allowed
// 
// description : 	Read/Write allowed for GaussianFitBG attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_GaussianFitBG_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_EnableAutoROI_allowed
// 
// description : 	Read/Write allowed for EnableAutoROI attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_EnableAutoROI_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_AutoROIMagFactor_allowed
// 
// description : 	Read/Write allowed for AutoROIMagFactor attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_AutoROIMagFactor_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_MaxIntensity_allowed
// 
// description : 	Read/Write allowed for MaxIntensity attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_MaxIntensity_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_CentroidX_allowed
// 
// description : 	Read/Write allowed for CentroidX attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_CentroidX_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_CentroidY_allowed
// 
// description : 	Read/Write allowed for CentroidY attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_CentroidY_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_VarianceX_allowed
// 
// description : 	Read/Write allowed for VarianceX attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_VarianceX_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_VarianceY_allowed
// 
// description : 	Read/Write allowed for VarianceY attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_VarianceY_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_CovarianceXY_allowed
// 
// description : 	Read/Write allowed for CovarianceXY attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_CovarianceXY_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_CorrelationXY_allowed
// 
// description : 	Read/Write allowed for CorrelationXY attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_CorrelationXY_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_SkewX_allowed
// 
// description : 	Read/Write allowed for SkewX attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_SkewX_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_SkewY_allowed
// 
// description : 	Read/Write allowed for SkewY attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_SkewY_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_SkewX2Y_allowed
// 
// description : 	Read/Write allowed for SkewX2Y attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_SkewX2Y_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_SkewXY2_allowed
// 
// description : 	Read/Write allowed for SkewXY2 attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_SkewXY2_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}

//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_XProfileCenter_allowed
// 
// description : 	Read/Write allowed for XProfileCenter attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_XProfileCenter_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_XProfileMag_allowed
// 
// description : 	Read/Write allowed for XProfileMag attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_XProfileMag_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_YProfileCenter_allowed
// 
// description : 	Read/Write allowed for YProfileCenter attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_YProfileCenter_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_YProfileMag_allowed
// 
// description : 	Read/Write allowed for YProfileMag attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_YProfileMag_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_GaussianFitCenterX_allowed
// 
// description : 	Read/Write allowed for GaussianFitCenterX attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_GaussianFitCenterX_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_GaussianFitCenterY_allowed
// 
// description : 	Read/Write allowed for GaussianFitCenterY attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_GaussianFitCenterY_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_GaussianFitVarianceX_allowed
// 
// description : 	Read/Write allowed for GaussianFitVarianceX attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_GaussianFitVarianceX_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_GaussianFitVarianceY_allowed
// 
// description : 	Read/Write allowed for GaussianFitVarianceY attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_GaussianFitVarianceY_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_GaussianFitCovarianceXY_allowed
// 
// description : 	Read/Write allowed for GaussianFitCovarianceXY attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_GaussianFitCovarianceXY_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_ROIImage_allowed
// 
// description : 	Read/Write allowed for ROIImage attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_ROIImage_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_EnableUserROI_allowed
// 
// description : 	Read/Write allowed for EnableUserROI attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_EnableUserROI_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_UserROIOriginX_allowed
// 
// description : 	Read/Write allowed for UserROIOriginX attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_UserROIOriginX_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_UserROIOriginY_allowed
// 
// description : 	Read/Write allowed for UserROIOriginY attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_UserROIOriginY_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_UserROIWidth_allowed
// 
// description : 	Read/Write allowed for UserROIWidth attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_UserROIWidth_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_UserROIHeight_allowed
// 
// description : 	Read/Write allowed for UserROIHeight attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_UserROIHeight_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_AutoROIOriginX_allowed
// 
// description : 	Read/Write allowed for AutoROIOriginX attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_AutoROIOriginX_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_AutoROIOriginY_allowed
// 
// description : 	Read/Write allowed for AutoROIOriginY attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_AutoROIOriginY_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_AutoROIWidth_allowed
// 
// description : 	Read/Write allowed for AutoROIWidth attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_AutoROIWidth_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_AutoROIHeight_allowed
// 
// description : 	Read/Write allowed for AutoROIHeight attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_AutoROIHeight_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_XProfileFWHM_allowed
// 
// description : 	Read/Write allowed for XProfileFWHM attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_XProfileFWHM_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_YProfileFWHM_allowed
// 
// description : 	Read/Write allowed for YProfileFWHM attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_YProfileFWHM_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_GaussianFitMajorAxisFWHM_allowed
// 
// description : 	Read/Write allowed for GaussianFitMajorAxisFWHM attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_GaussianFitMajorAxisFWHM_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_GaussianFitMinorAxisFWHM_allowed
// 
// description : 	Read/Write allowed for GaussianFitMinorAxisFWHM attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_GaussianFitMinorAxisFWHM_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_XProfileFitted_allowed
// 
// description : 	Read/Write allowed for XProfileFitted attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_XProfileFitted_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_XProfileError_allowed
// 
// description : 	Read/Write allowed for XProfileError attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_XProfileError_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_YProfileFitted_allowed
// 
// description : 	Read/Write allowed for YProfileFitted attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_YProfileFitted_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_YProfileError_allowed
// 
// description : 	Read/Write allowed for YProfileError attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_YProfileError_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_Rotation_allowed
// 
// description : 	Read/Write allowed for Rotation attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_Rotation_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_HorizontalFlip_allowed
// 
// description : 	Read/Write allowed for HorizontalFlip attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_HorizontalFlip_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_GammaCorrection_allowed
// 
// description : 	Read/Write allowed for GammaCorrection attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_GammaCorrection_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_BitsPerPixel_allowed
// 
// description : 	Read/Write allowed for BitsPerPixel attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_BitsPerPixel_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_NbNoiseImage_allowed
// 
// description : 	Read/Write allowed for NbNoiseImage attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_NbNoiseImage_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_MeanNoiseImage_allowed
// 
// description : 	Read/Write allowed for MeanNoiseImage attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_MeanNoiseImage_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_PixelSizeX_allowed
// 
// description : 	Read/Write allowed for PixelSizeX attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_PixelSizeX_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_PixelSizeY_allowed
// 
// description : 	Read/Write allowed for PixelSizeY attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_PixelSizeY_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_OpticalMagnification_allowed
// 
// description : 	Read/Write allowed for OpticalMagnification attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_OpticalMagnification_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_AutoROIFound_allowed
// 
// description : 	Read/Write allowed for AutoROIFound attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_AutoROIFound_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_XProfileFitConverged_allowed
// 
// description : 	Read/Write allowed for XProfileFitConverged attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_XProfileFitConverged_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_YProfileFitConverged_allowed
// 
// description : 	Read/Write allowed for YProfileFitConverged attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_YProfileFitConverged_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_GaussianFitConverged_allowed
// 
// description : 	Read/Write allowed for GaussianFitConverged attribute.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_GaussianFitConverged_allowed(Tango::AttReqType type)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}

//=================================================
//		Commands Allowed Methods
//=================================================

//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_Start_allowed
// 
// description : 	Execution allowed for Start command.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_Start_allowed(const CORBA::Any &any)
{
	if (get_state() == Tango::RUNNING)
	{
		//	End of Generated Code

		//	Re-Start of Generated Code
		return false;
	}
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_Stop_allowed
// 
// description : 	Execution allowed for Stop command.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_Stop_allowed(const CORBA::Any &any)
{
	if (get_state() == Tango::STANDBY)
	{
		//	End of Generated Code

		//	Re-Start of Generated Code
		return false;
	}
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_Process_allowed
// 
// description : 	Execution allowed for Process command.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_Process_allowed(const CORBA::Any &any)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_SaveCurrentSettings_allowed
// 
// description : 	Execution allowed for SaveCurrentSettings command.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_SaveCurrentSettings_allowed(const CORBA::Any &any)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_StartLearnNoise_allowed
// 
// description : 	Execution allowed for StartLearnNoise command.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_StartLearnNoise_allowed(const CORBA::Any &any)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_StopLearnNoise_allowed
// 
// description : 	Execution allowed for StopLearnNoise command.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_StopLearnNoise_allowed(const CORBA::Any &any)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}
//+----------------------------------------------------------------------------
//
// method : 		ImgBeamAnalyzer::is_GetVersionNumber_allowed
// 
// description : 	Execution allowed for GetVersionNumber command.
//
//-----------------------------------------------------------------------------
bool ImgBeamAnalyzer::is_GetVersionNumber_allowed(const CORBA::Any &any)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
	return true;
}

}	// namespace ImgBeamAnalyzer_ns
