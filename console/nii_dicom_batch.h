//
//

#ifndef MRIpro_nii_batch_h
#define MRIpro_nii_batch_h

#ifdef  __cplusplus
extern "C" {
#endif

#include <stdbool.h> //requires VS 2015 or later
#include <string.h>
#ifndef USING_R
#include "nifti1.h"
#endif
#include "nii_dicom.h"

#ifdef USING_R
    struct TDicomSeries {
        std::string name;
        TDICOMdata representativeData;
        std::vector<std::string> files;
    };
#endif

#define kNAME_CONFLICT_SKIP 0 //0 = write nothing for a file that exists with desired name
#define kNAME_CONFLICT_OVERWRITE 1 //1 = overwrite existing file with same name
#define kNAME_CONFLICT_ADD_SUFFIX 2 //default 2 = write with new suffix as a new file

#define MAX_NUM_SERIES 16

    struct TDCMopts {
        bool isSaveNRRD, isOneDirAtATime, isRenameNotConvert, isMaximize16BitRange, isSave3D, isGz, isPipedGz, isFlipY,  isCreateBIDS, isSortDTIbyBVal, isAnonymizeBIDS, isOnlyBIDS, isCreateText, isIgnoreDerivedAnd2D, isPhilipsFloatNotDisplayScaling, isTiltCorrect, isRGBplanar, isOnlySingleFile, isForceStackDCE, isRotate3DAcq, isCrop;
        int isForceStackSameSeries, nameConflictBehavior, isVerbose, isProgress, compressFlag, dirSearchDepth, gzLevel; //support for compressed data 0=none,
        char filename[512], outdir[512], indir[512], pigzname[512], optsname[512], indirParent[512], imageComments[24];
        float seriesNumber[MAX_NUM_SERIES];
        long numSeries;
#ifdef USING_R
        bool isScanOnly;
        void *imageList;
        std::vector<TDicomSeries> series;

        // Used when sorting a directory
        std::vector<std::string> sourcePaths;
        std::vector<std::string> targetPaths;
        std::vector<std::string> ignoredPaths;
#endif
    };
    void saveIniFile (struct TDCMopts opts);
    void setDefaultOpts (struct TDCMopts *opts, const char * argv[]); //either "setDefaultOpts(opts,NULL)" or "setDefaultOpts(opts,argv)" where argv[0] is path to search
    void readIniFile (struct TDCMopts *opts, const char * argv[]);
    int nii_saveNIIx(char * niiFilename, struct nifti_1_header hdr, unsigned char* im, struct TDCMopts opts);
    //void readIniFile (struct TDCMopts *opts);
    int nii_loadDir(struct TDCMopts *opts);
    //void nii_SaveBIDS(char pathoutname[], struct TDICOMdata d, struct TDCMopts opts, struct TDTI4D *dti4D, struct nifti_1_header *h, const char * filename);
    void nii_SaveBIDS(char pathoutname[], struct TDICOMdata d, struct TDCMopts opts, struct nifti_1_header *h, const char * filename);
    int nii_createFilename(struct TDICOMdata dcm, char * niiFilename, struct TDCMopts opts);
    void  nii_createDummyFilename(char * niiFilename, struct TDCMopts opts);
    //void findExe(char name[512], const char * argv[]);
#ifdef  __cplusplus
}
#endif

#endif
