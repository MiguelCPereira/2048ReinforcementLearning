#include<direct.h>
#include "LinearQNet.h"

LinearQNet::LinearQNet(int inputSize, int hiddenSize, int outputSize)
    //: m_LinearA(inputSize, hiddenSize)
	//, m_LinearB(hiddenSize, outputSize)
{
}

//torch::Tensor LinearQNet::Forward(torch::Tensor x)
//{
//    x = torch::relu(m_LinearA(x));
//    x = m_LinearB(x);
//	
//    return x;
//}

void LinearQNet::Save(std::string fileName) const
{
    std::string modelFolderPath = "./model";

	// If the model folder doesn't exist yet
    struct stat buffer;
    if(stat(modelFolderPath.c_str(), &buffer) == 0)
    {
	    // Create it
        _mkdir(modelFolderPath.c_str());
    }

    fileName = modelFolderPath + fileName;

    //torch::serialize::OutputArchive outputArchive;
    //torch::save(outputArchive);
    //outputArchive.save_to(fileName);
}

