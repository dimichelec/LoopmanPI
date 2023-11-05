/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   background_png;
    const int            background_pngSize = 100242;

    extern const char*   knob_png;
    const int            knob_pngSize = 12707;

    extern const char*   loopButton_png;
    const int            loopButton_pngSize = 10823;

    extern const char*   redoButton_png;
    const int            redoButton_pngSize = 3863;

    extern const char*   stopButton_png;
    const int            stopButton_pngSize = 8985;

    extern const char*   undoButton_png;
    const int            undoButton_pngSize = 3932;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 6;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Points to the start of a list of resource filenames.
    extern const char* originalFilenames[];

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
}
