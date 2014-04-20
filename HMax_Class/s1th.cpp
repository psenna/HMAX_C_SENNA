#include "s1th.h"

S1Th::S1Th(QObject *parent) :
    QThread(parent)
{
}

void S1Th::run(){
    this->gaborFilterResult = new std::vector<S1_T>;
    // Caso os filtros tenham que ser criados.
    if(filters != NULL){
        int quantidade = orientation->size() * sizes->size();
        gaborFilterResult->resize(quantidade);

        filters = new std::vector<cv::Mat>;
        filters->resize(quantidade);

        std::vector<cv::Mat>::iterator filter = filters->begin();
        std::vector<S1_T>::iterator result = gaborFilterResult->begin();
        std::vector<double>::iterator orient = orientation->begin();

        for(int i = 0; i < orientation->size(); i++){
            std::vector<int>::iterator tamanho = sizes->begin();
            std::vector<double>::iterator lamb = lambda->begin();
            std::vector<double>::iterator sig = sigma->begin();
            std::vector<double>::iterator gam = gama->begin();

            for(int j = 0; j < sizes->size(); j++){
                *filter = cv::getGaborKernel(cv::Size(*tamanho, *tamanho), *sig, *orient, *lamb, *gam, 0, CV_64F);
                result->tamanho = *tamanho;
                result->orientation = *orient;

                cv::filter2D(image, result->imgFiltrada, CV_64F, *filter);

                result++;
                filter++;
                tamanho++;
                lamb++;
                sig++;
                gam++;
            }
            orient++;
        }

    } else {
        /// Quando os filtros ja foram calculados e usados em outras imagens.
        gaborFilterResult->resize(filters->size());
        std::vector<cv::Mat>::iterator filter = filters->begin();
        std::vector<S1_T>::iterator result = gaborFilterResult->begin();
        std::vector<double>::iterator orient = orientation->begin();

        for(int i = 0; i < orientation->size(); i++){
            std::vector<int>::iterator tamanho = sizes->begin();
            for(int j = 0; j < sizes->size(); j++){
                result->tamanho = *tamanho;
                result->orientation = *orient;

                cv::filter2D(image, result->imgFiltrada, CV_64F, *filter);

                result++;
                filter++;
                tamanho++;
            }
            orient++;
        }
    }
}
