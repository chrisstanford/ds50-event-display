#ifndef PMT_hh
#define PMT_hh

#include "TEveGeoShape.h"

#include "TGeoSphere.h"
#include "TGeoMatrix.h"
#include "TGeoScaledShape.h"
#include "TGeoTube.h"
#include "TGeoCompositeShape.h"

namespace display {
  class PMT : public TEveGeoShape {
  public:
    PMT(std::string detector, int ch_id) : size(80.) {
      if (detector=="lsv"||detector=="wt") {
	fGSphere         = new TGeoSphere("fGSphere",0.,1.5*size,0.,180.);
	fGScale          = new TGeoScale(1.,1.,0.5);
	fGEllipsoid      = new TGeoScaledShape("fGEllipsoid", fGSphere, fGScale);
	fGTube           = new TGeoTube("fGTube",0, 0.6*size,size);
	fGTranslation    = new TGeoTranslation("fGTranslation",0,0,size);fGTranslation->RegisterYourself();
	fGCompositeShape = new TGeoCompositeShape("fGCompositeShape", "(fGEllipsoid + fGTube:fGTranslation)");
	SetName(Form("%s_channel_%d",detector.c_str(),ch_id));
	SetShape(fGCompositeShape);
      } else if (detector=="tpc") {
	fGSphere         = new TGeoSphere("fGSphere",0.,size,0.,90.);
	fGTube           = new TGeoTube("fGTube",0, 0.6*size,size);
	fGTranslation    = new TGeoTranslation("fGTranslation",0,0,1.01*size);fGTranslation->RegisterYourself();
	fGCompositeShape = new TGeoCompositeShape("fGCompositeShape", "(fGSphere + fGTube:fGTranslation)");
	SetName(Form("%s_channel_%d",detector.c_str(),ch_id));
	SetShape(fGCompositeShape);
      }
    }
  private:
    double size;
      
    TGeoSphere*         fGSphere;
    TGeoScale*          fGScale;
    TGeoScaledShape*    fGEllipsoid;
    TGeoTube*           fGTube;
    TGeoTranslation*    fGTranslation;
    TGeoCompositeShape* fGCompositeShape;
  };
}
#endif
