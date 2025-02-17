## About

dcm2niix attempts to convert Siemens DICOM format images to NIfTI. This page describes some vendor-specific details.

## Vida XA10 and XA11

The Siemens Vida introduced the new XA10 DICOM format. Users are strongly encouraged to export data using the "Enhanced" format and to not use any of the "Anonymize" features on the console. The consequences of these options is discussed in detail in [issue 236](https://github.com/rordenlab/dcm2niix/issues/236). In brief, the Vida can export to enhanced, mosaic or classic 2D. Note that the mosaics are considered secondary capture images intended for quality assurance only. The mosaic scans lack several "Type 1" DICOM properties, necessarily limiting conversion. The non-mosaic 2D enhanced DICOMs are compact and efficient, but appear to have limited details relative to the enhanced output. Finally, each of the formats (enhanced, mosaic, classic) can be exported as anonymized. The Siemens console anonymization of current XA10A (Fall 2018) strips many useful tags. Siemens suggests "the use an offline/in-house anonymization software instead." Another limitation of the current XA10A format is that it retains no versioning details for software and hardware stepping, despite the fact that the data format is rapidly evolving. If you use a Vida, you are strongly encouraged to log every hardware or software upgrade to allow future analyses to identify and regress out any effects of these modifications.  Since the XA10A format does not have a CSA header, dcm2niix will attempt to use the new private DICOM tags to populate the BIDS file. These tags are described in [issue 240](https://github.com/rordenlab/dcm2niix/issues/240).

When creating enhanced DICOMs diffusion information is provided in public tags. Based on a limited sample, it seems that classic DICOMs do not store diffusion data for XA10, and use private tags for [XA11](https://www.nitrc.org/forum/forum.php?thread_id=10013&forum_id=4703).

Public Tags
```
(0018,9089) FD -0.20\-0.51\-0.83 #DiffusionGradientOrientation
(0018,9087) FD 1000 #DiffusionBValue

```

Private Tags
```
(0019,100c) IS 1000 #SiemensDiffusionBValue
(0019,100e) FD -0.20\-0.51\-0.83 #SiemensDiffusionGradientOrientation

```

In theory, the public DICOM tag 'Frame Acquisition Date Time' (0018,9074) and the private tag 'Time After Start' (0021,1104) should each allow one to infer slice timing. The tag 0018,9074 uses the DT (date time) format, for example "20190621095520.330000" providing the YYYYYMMDDHHMMSS. Unfortunately, the Siemens de-identification routines will scramble these values, as time of data could be considered an identifiable attribute. The tag 0021,1104 is saved in DS (decimal string) format, for example "4.635" reporting the number of seconds since acquisition started. Be aware that some [Siemens Vida multi-band sequences](https://github.com/rordenlab/dcm2niix/issues/303) appear to fill these tags with the single-band times rather than the actual acquisition times. Therefore, neither of these two methods is perfectly reliable in determining slice timing.

## CSA Header

Many crucial Siemens parameters are stored in the [proprietary CSA header](http://nipy.org/nibabel/dicom/siemens_csa.html). This has a binary section that allows quick reading for many useful parameters. It also includes an ASCII text portion that includes a lot of information but is slow to parse and poorly curated. Be aware that Siemens Vida scanners do not generate a CSA header.

## Slice Timing

For Siemens images created with software versions B15 through E11, the proprietary [CSA Image Header (0029,1010)](https://nipy.org/nibabel/dicom/siemens_csa.html) contains the array MosaicRefAcqTimes that provides [slice timing](https://www.mccauslandcenter.sc.edu/crnl/tools/stc). Earlier Siemens Software (e.g. A25 through B13) sometimes populates the tag sSliceArray.ucMode in the [CSA Series Header (0029, 1020)](https://nipy.org/nibabel/dicom/siemens_csa.html) where the values [1, 2, and 4](https://github.com/xiangruili/dicm2nii/issues/18) correspond to Ascending, Descending and Interleaved acquisitions. Therefore dcm2niix typically is able to provide accurate slice timing information for non-Vida datasets (the prior section describes Vida slice timing issues seen with the XA software series). Some Siemens DICOMs stroe slice timings in the private tag [0019,1029](https://github.com/rordenlab/dcm2niix/issues/296). In theory, this could be used when the CSA header is missing. For archival studies, be aware that some sequences [incorrectly reported slice timing](https://github.com/rordenlab/dcm2niix/issues/126). The [SPM slice timing wiki](https://en.wikibooks.org/w/index.php?title=SPM/Slice_Timing&stable=0#Siemens_scanners) provides further information on Siemens slice timing.

## Total Readout Time

One often wants to determine [echo spacing, bandwidth, ](https://support.brainvoyager.com/brainvoyager/functional-analysis-preparation/29-pre-processing/78-epi-distortion-correction-echo-spacing-and-bandwidth) and total read-out time for EPI data so they can be undistorted. The [Siemens validation dataset](https://github.com/neurolabusc/dcm_qa/tree/master/In/TotalReadoutTime) demonstrates that dcm2niix can accurately report these parameters - the included notes and spreadsheet describe this in more detail.

## Diffusion Tensor Notes

Diffusion specific parameters are described on the [NA-MIC](https://www.na-mic.org/wiki/NAMIC_Wiki:DTI:DICOM_for_DWI_and_DTI#Private_vendor:_Siemens) website. Gradient vectors are reported with respect to the scanner bore, and dcm2niix will attempt to re-orient these to [FSL format](http://justinblaber.org/brief-introduction-to-dwmri/) [bvec files](https://fsl.fmrib.ox.ac.uk/fsl/fslwiki/FDT/FAQ#What_conventions_do_the_bvecs_use.3F). For the Vida, see the Vida section for specific diffusion details.

## Sample Datasets

 - [Slice timing dataset](httphttps://www.nitrc.org/plugins/mwiki/index.php/dcm2nii:MainPage#Slice_timing_corrections://www.nitrc.org/plugins/mwiki/index.php/dcm2nii:MainPage).
 - [A validation dataset for dcm2niix commits](https://github.com/neurolabusc/dcm_qa).
 - [A mixture of GE and Siemens data](https://github.com/neurolabusc/dcm_qa_nih).
 - [DTI examples](https://www.nitrc.org/plugins/mwiki/index.php/dcm2nii:MainPage#Diffusion_Tensor_Imaging).
 - [Archival (old) examples](https://www.nitrc.org/plugins/mwiki/index.php/dcm2nii:MainPage#Archival_MRI).
 - [Unusual examples](https://www.nitrc.org/plugins/mwiki/index.php/dcm2nii:MainPage#Unusual_MRI).

