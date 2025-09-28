#Fill pathToAssets in line 2 and 3 with the path of the extracted zip folder (don't forget to do C:/ not C:\)
message(STATUS "THIRD_PARTY_ASSETS = pathToAssets")
set(THIRD_PARTY_ASSETS "pathToAssets")

#Include Folder
file(INSTALL "${THIRD_PARTY_ASSETS}/Include/" DESTINATION "${CURRENT_PACKAGES_DIR}/Include")
#Install libraries
file(INSTALL "${THIRD_PARTY_ASSETS}/Lib/" DESTINATION "${CURRENT_PACKAGES_DIR}/Lib")
#DirectX11 redestribute
file(INSTALL "${THIRD_PARTY_ASSETS}/Redistribute/" DESTINATION "${CURRENT_PACKAGES_DIR}/Redistribute")
#Models
file(INSTALL "${THIRD_PARTY_ASSETS}/Models/" DESTINATION "${CURRENT_PACKAGES_DIR}/Models")