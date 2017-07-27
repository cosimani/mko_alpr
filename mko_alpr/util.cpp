#include "util.h"

Util* Util::instance = NULL;

Util::Util(QObject *parent) :
    QObject(parent)
{
}

Util *Util::getInstance()
{
    if(!instance)
    {
        instance = new Util();
        LOG_INF("Util: successfully initialized");
    }
    return instance;
}

QString Util::toCamelCase(const QString& s)
{
    QStringList parts = s.split(' ', QString::SkipEmptyParts);
    for (int i = 0; i < parts.size(); ++i)
        parts[i].replace(0, 1, parts[i][0].toUpper());

    return parts.join(" ");
}

Util::~Util()
{
    delete instance;
}

#ifdef DEBUG_OSI
void Util::publicarInfoAlprConfig( alpr::Config *config )
{
    qDebug() << "Informacion del objeto alpr::Config ////////////////";
    qDebug() << "loaded" << config->loaded;
    qDebug() << "config_file_path" << config->config_file_path.c_str();
    qDebug() << "country" << config->country.c_str();
//    int detector;
//    float detection_iteration_increase;
//    int detectionStrictness;
    qDebug() << "maxPlateWidthPercent" << config->maxPlateWidthPercent;
    qDebug() << "maxPlateHeightPercent" << config->maxPlateHeightPercent;
    qDebug() << "maxDetectionInputWidth" << config->maxDetectionInputWidth;
    qDebug() << "maxDetectionInputHeight" << config->maxDetectionInputHeight;
//    float contrastDetectionThreshold;
//    bool skipDetection;
//    std::string detection_mask_image;
    qDebug() << "analysis_count" << config->analysis_count;
//    bool auto_invert;
//    bool always_invert;
//    std::string prewarp;
    qDebug() << "maxPlateAngleDegrees" << config->maxPlateAngleDegrees;
    qDebug() << "minPlateSizeWidthPx" << config->minPlateSizeWidthPx;
    qDebug() << "minPlateSizeHeightPx" << config->minPlateSizeHeightPx;
//    bool multiline;
    qDebug() << "plateWidthMM" << config->plateWidthMM;
    qDebug() << "plateHeightMM" << config->plateHeightMM;
//    std::vector<float> charHeightMM;
//    std::vector<float> charWidthMM;
//    float avgCharHeightMM;
//    float avgCharWidthMM;
//    float charWhitespaceTopMM;
//    float charWhitespaceBotMM;
//    float charWhitespaceBetweenLinesMM;
//    int templateWidthPx;
//    int templateHeightPx;
//    int ocrImageWidthPx;
//    int ocrImageHeightPx;
//    int stateIdImageWidthPx;
//    int stateIdimageHeightPx;
//    float charAnalysisMinPercent;
//    float charAnalysisHeightRange;
//    float charAnalysisHeightStepSize;
//    int charAnalysisNumSteps;
//    float plateLinesSensitivityVertical;
//    float plateLinesSensitivityHorizontal;
//    float segmentationMinSpeckleHeightPercent;
//    int segmentationMinBoxWidthPx;
//    float segmentationMinCharHeightPercent;
//    float segmentationMaxCharWidthvsAverage;
    qDebug() << "detectorFile" << config->detectorFile.c_str();
    qDebug() << "ocrLanguage" << config->ocrLanguage.c_str();
//    int ocrMinFontSize;
//    bool mustMatchPattern;
//    float postProcessMinConfidence;
//    float postProcessConfidenceSkipLevel;
    qDebug() << "postProcessMinCharacters" << config->postProcessMinCharacters;
    qDebug() << "postProcessMaxCharacters" << config->postProcessMaxCharacters;
    qDebug() << "postProcessRegexLetters" << config->postProcessRegexLetters.c_str();
    qDebug() << "postProcessRegexNumbers" << config->postProcessRegexNumbers.c_str();
//    bool debugGeneral;
//    bool debugTiming;
//    bool debugPrewarp;
//    bool debugDetector;
//    bool debugStateId;
//    bool debugPlateLines;
//    bool debugPlateCorners;
//    bool debugCharSegmenter;
//    bool debugCharAnalysis;
//    bool debugColorFiler;
//    bool debugOcr;
//    bool debugPostProcess;
//    bool debugShowImages;
//    bool debugPauseOnFrame;

//    void setDebug(bool value);
    qDebug() << "getKeypointsRuntimeDir()" << config->getKeypointsRuntimeDir().c_str();
    qDebug() << "getCascadeRuntimeDir()" << config->getCascadeRuntimeDir().c_str();
    qDebug() << "getPostProcessRuntimeDir()" << config->getPostProcessRuntimeDir().c_str();
    qDebug() << "getTessdataPrefix()" << config->getTessdataPrefix().c_str();
    qDebug() << "runtimeBaseDir" << config->runtimeBaseDir.c_str();

//    std::vector<std::string> loaded_countries;

//    bool setCountry(std::string country);
}
#endif
