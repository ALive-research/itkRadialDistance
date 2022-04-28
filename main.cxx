// TCLAP includes
#include <tclap/ValueArg.h>
#include <tclap/ArgException.h>
#include <tclap/CmdLine.h>

//ITK includes
#include <itkImageFileWriter.h>
#include <itkImageRegionIterator.h>

// STD includes
#include <cstdlib>
#include <string>
#include <cmath>

// ===========================================================================
// Entry point
// ===========================================================================
int main(int argc, char **argv)
{
  // =========================================================================
  // Command-line variables
  // =========================================================================
  std::string output;
  unsigned int inputSize;
  float radius;

  // =========================================================================
  // Parse arguments
  // =========================================================================
  try
    {
    TCLAP::CmdLine cmd("itkRadialDistance");

    TCLAP::ValueArg<std::string> outputArgument("o", "output", "Output file", true, "None", "string");
    TCLAP::ValueArg<unsigned int> sizeArgument("z", "size", "Image size in voxels (per axis)", false, 100, "unsigned int");
    TCLAP::ValueArg<float> radiusArgument("r", "radius", "Radius of the sphere", false, 1.0f, "float");

    cmd.add(sizeArgument);
    cmd.add(outputArgument);
    cmd.add(radiusArgument);

    cmd.parse(argc,argv);

    output = outputArgument.getValue();
    inputSize = sizeArgument.getValue();

    if (radius < 0.0f)
    {
      std::cerr << "Error: radius must be positive" << std::endl;
      return EXIT_FAILURE;
    }

    }
  catch(TCLAP::ArgException &e)
    {
    std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
    }

  // =========================================================================
  // ITK definitions
  // =========================================================================
  using DistanceImageType = itk::Image<float, 3>;
  using DistanceWriterType = itk::ImageFileWriter<DistanceImageType>;
  using DistanceImageIterator = itk::ImageRegionIterator<DistanceImageType>;

  // =========================================================================
  // Generate the image
  // =========================================================================
  itk::Index<3> start;
  start.Fill(0);
  itk::Size<3> size;
  size.Fill(inputSize);
  itk::ImageRegion<3> region(start, size);

  auto image = DistanceImageType::New();
  image->SetRegions(region);
  image->Allocate();
  image->FillBuffer(0);

  // =========================================================================
  // Populate the image with the distance
  // =========================================================================
  float center[3] = {inputSize/2.0f};
  DistanceImageIterator it(image, image->GetLargestPossibleRegion());
  while(!it.IsAtEnd())
  {
    DistanceImageType::PointType point;
    image->TransformIndexToPhysicalPoint(it.GetIndex(), point);

    float distance = sqrt(pow(point[0]-center[0], 2) +
                          pow(point[1]-center[1], 2) +
                          pow(point[2]-center[2], 2));

    it.Set(distance-radius);

   ++it;
  }

  // =========================================================================
  // Output writing
  // =========================================================================
  auto writer = DistanceWriterType::New();
  writer->SetFileName(output);
  writer->SetInput(image);

  try
  {
    writer->Update();
  }
  catch (const itk::ExceptionObject & excp)
  {
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
