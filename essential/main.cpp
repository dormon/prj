#include<iostream>
#include<sstream>
#include<map>
#include<set>
#include<vector>
#include<typeinfo>
#include<memory>
#include<cmath>
#include<cstdlib>

class Mat3{
  public:
    float data[3*3];
};

class Vec3{
  public:
    float data[3];
};

class RansacPoint{
  public:
};

class RansacPoints{
  public:
    std::set<std::shared_ptr<RansacPoint>>points;
    RansacPoints getRandomSamplePoints(size_t nofPoints)const;
    virtual ~RansacPoints(){}
};

class RansacModel{
  public:
    RansacModel();
    virtual void fit(RansacPoints const&points) = 0;
    virtual size_t getNofPointsRequiredForModelConstruction()const = 0;
    virtual float getDistance(RansacPoint const&point)const = 0;
    static size_t getNofIteration(
        size_t nofPointsRequiredForModelConstruction,
        float  desiredProbabilityOfFindingSolution  ,
        float  probabilityThatAPointIsAnOutlier     ){
      float const probabilityThatAPointIsAnInlier = 1.f - probabilityThatAPointIsAnOutlier;
      float const probabilityThatAllPointsInSampleAreInliers = std::pow(probabilityThatAPointIsAnInlier,static_cast<float>(nofPointsRequiredForModelConstruction));
      float const probabilityThatAtLeastOnePointInSampleIsOutlier = 1.f - probabilityThatAllPointsInSampleAreInliers;
      float const probabilityThatASampleIsContaminated = probabilityThatAtLeastOnePointInSampleIsOutlier;
      float const probabilityThatRansacFails = 1.f - desiredProbabilityOfFindingSolution;
      return static_cast<size_t>(std::ceil(std::log(probabilityThatRansacFails)/std::log(probabilityThatASampleIsContaminated)));
    }
};

void ransac(
    RansacModel* const&model,
    RansacPoints const&points,
    float desiredProbability,
    float probabilityThatAPointIsAnOutlier,
    float inlierOutlierThreshold){
  auto const requiredPoints = model->getNofPointsRequiredForModelConstruction();
  auto const nofIteration = RansacModel::getNofIteration(
      requiredPoints,
      desiredProbability,
      probabilityThatAPointIsAnOutlier);

  RansacPoints bestSample;
  std::set<RansacPoint> bestModelInliers;
  for(size_t sampleIndex = 0; sampleIndex < nofIteration; ++sampleIndex){
    auto sample = points.getRandomSamplePoints(requiredPoints);
    model->fit(sample);
    RansacPoints inliers;
    for(auto const&point:points.points){
      if(sample.points.find(point) != sample.points.end())continue;
      auto const pointNotInSample = point;
      float const modelToPointDistance = model->getDistance(*pointNotInSample);
      if(modelToPointDistance >= inlierOutlierThreshold)continue;
      auto const inlier = pointNotInSample;
      inliers.points.insert(inlier);
    }
  }

}

//
//Mat3 getEssentialMatrix(
//    std::vector<Vec3>const&a,
//    std::vector<Vec3>const&b){
//
//}
//
//void singularValueDecomposition(
//    Mat3      &u    ,
//    Mat3      &sigma,
//    Mat3      &vt   ,
//    Mat3 const&input){
//}



int main(){

  return 0;
}
