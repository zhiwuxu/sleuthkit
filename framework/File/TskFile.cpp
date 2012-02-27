/*
 * The Sleuth Kit
 *
 * Contact: Brian Carrier [carrier <at> sleuthkit [dot] org]
 * Copyright (c) 2010-2011 Basis Technology Corporation. All Rights
 * reserved.
 *
 * This software is distributed under the Common Public License 1.0
 */

/**
 * \file TskFile.cpp
 * Contains the implementation for the TskFile class.
 */

// System includes
#include <sstream>

// Framework includes
#include "TskFile.h"
#include "Services/TskServices.h"

/**
 * Delete the TskFile object.
 */
TskFile::~TskFile(void)
{
}

/**
 * 
 */
void TskFile::initialize()
{
    TskImgDB * imgDB = &TskServices::Instance().getImgDB();

    // XXX We never check the return value...
    if (imgDB != NULL)
        imgDB->getFileRecord(m_id, m_fileRecord);
}

/**
 * What is this files id?
 */
uint64_t TskFile::id() const
{
    return m_id;
}

/**
 * What is this files type id?
 */
int TskFile::typeId() const
{
    return m_fileRecord.typeId;
}

/**
 * What is this files name?
 */
std::string TskFile::name() const
{
    return m_fileRecord.name;
}

/**
 * What is this files extension?
 */
std::string TskFile::extension() const
{
    return m_fileRecord.name.substr(m_fileRecord.name.find_last_of(".") + 1);
}

/**
 * What is this files parent file id?
 */
uint64_t TskFile::parentFileId() const
{
    return m_fileRecord.parentFileId;
}

/**
 * What is this files directory type?
 */
int TskFile::dirType() const
{
    return m_fileRecord.dirType;
}
/**
 * What is this files metadata type?
 */
int TskFile::metaType() const
{
    return m_fileRecord.metaType;
}

/**
 * What are this files directory flags?
 */
int TskFile::dirFlags() const
{
    return m_fileRecord.dirFlags;
}

/**
 * What are this files metadata flags?
 */
int TskFile::metaFlags() const
{
    return m_fileRecord.metaFlags;
}

/**
 * What is this files size?
 */
uint64_t TskFile::size() const
{
    return m_fileRecord.size;
}

/**
 * What is this files change time?
 */
int TskFile::ctime() const
{
    return m_fileRecord.ctime;
}

/**
 * What is this files creation time?
 */
int TskFile::crtime() const
{
    return m_fileRecord.crtime;
}

/**
 * What is this files access time?
 */
int TskFile::atime() const
{
    return m_fileRecord.atime;
}

/**
 * What is this files modify time?
 */
int TskFile::mtime() const
{
    return m_fileRecord.mtime;
}

/**
 * What is this files mode?
 */
int TskFile::mode() const
{
    return m_fileRecord.mode;
}

/**
 * What is this files user id?
 */
int TskFile::uid() const
{
    return m_fileRecord.uid;
}

/**
 * What is this files group id?
 */
int TskFile::gid() const
{
    return m_fileRecord.gid;
}

/**
 * What is this files status?
 */
int TskFile::status() const
{
    return m_fileRecord.status;
}

/*
 * What is this files full path
 */
std::string TskFile::fullPath() const
{
    return m_fileRecord.fullPath;
}

// Get the file hash
std::string TskFile::getHash(TskImgDB::HASH_TYPE hashType) const
{
    switch (hashType) {
    case TskImgDB::MD5:
        return m_fileRecord.md5;
        break;
    case TskImgDB::SHA1:
        return m_fileRecord.sha1;
        break;
    case TskImgDB::SHA2_256:
        return m_fileRecord.sha2_256;
        break;
    case TskImgDB::SHA2_512:
        return m_fileRecord.sha2_512;
        break;
    };
    return "";
}

// Set the file hash
void TskFile::setHash(TskImgDB::HASH_TYPE hashType, const std::string hash)
{
    switch (hashType) {
    case TskImgDB::MD5:
        m_fileRecord.md5 = hash;
        break;
    case TskImgDB::SHA1:
        m_fileRecord.sha1 = hash;
        break;
    case TskImgDB::SHA2_256:
        m_fileRecord.sha2_256 = hash;
        break;
    case TskImgDB::SHA2_512:
        m_fileRecord.sha2_512 = hash;
        break;
    };
    if (TskServices::Instance().getImgDB().setHash(m_fileRecord.fileId, hashType, hash)) {
        throw TskException("setHash failed.");
    }
}

/// Set the file status
void TskFile::setStatus(TskImgDB::FILE_STATUS status)
{
    m_fileRecord.status = status;
    TskServices::Instance().getImgDB().updateFileStatus(id(), status);
}

/**
 * Create a new artifact with the given type id
 * @param artifactTypeID type id
 * @returns the new artifact
 */
TskBlackboardArtifact TskFile::newArtifact(int artifactTypeID){
    return TskServices::Instance().getBlackboard().newBlackboardArtifact(artifactTypeID, m_id);
}

/**
 * Create a new artifact with the given type
 * @param type artifact type
 * @returns the new artifact
 */
TskBlackboardArtifact TskFile::newArtifact(ARTIFACT_TYPE type){
    return TskServices::Instance().getBlackboard().newBlackboardArtifact(type, m_id);
}

/**
 * Get all artifacts associated with this file with the given type name
 * @param artifactTypeName type name
 * @returns all matching artifacts
 */
vector<TskBlackboardArtifact> TskFile::getArtifacts(string artifactTypeName){
    return TskServices::Instance().getBlackboard().getBlackboardArtifacts(artifactTypeName, m_id);
}

/**
 * Get all artifacts associated with this file with the given type id
 * @param artifactTypeid type id
 * @returns all matching artifacts
 */
vector<TskBlackboardArtifact> TskFile::getArtifacts(int artifactTypeID){
    return TskServices::Instance().getBlackboard().getBlackboardArtifacts(artifactTypeID, m_id);
}

/**
 * Get all artifacts associated with this file with the given type
 * @param type artifact type
 * @returns all matching artifacts
 */
vector<TskBlackboardArtifact> TskFile::getArtifacts(ARTIFACT_TYPE type){
    return TskServices::Instance().getBlackboard().getBlackboardArtifacts(type, m_id);
}

/**
 * Get all artifacts associated with this file
 * @returns all artifacts
 */
vector<TskBlackboardArtifact> TskFile::getAllArtifacts(){
    stringstream str;
    str << "WHERE artifact_id = " << m_id;
    return TskServices::Instance().getBlackboard().getMatchingArtifacts(str.str());
}

/**
 * Get the general info artifact for this file
 * @param type artifact type
 * @returns all matching artifacts
 */
TskBlackboardArtifact TskFile::getGenInfo(){
    TskBlackboard& blackboard = TskServices::Instance().getBlackboard();

    vector<TskBlackboardArtifact> artifacts;
    TskBlackboardArtifact genInfo;
    artifacts = getArtifacts(TSK_ART_GEN_INFO);

    if(artifacts.size() == 0)
        genInfo = newArtifact(TSK_ART_GEN_INFO);
    else
        genInfo = artifacts[0];

    return genInfo;
}

/**
 * Add an attribute to the general info artifact for this file
 * @param attribute attribute to be added
 */
void TskFile::addGenInfoAttribute(TskBlackboardAttribute attr){
    getGenInfo().addAttribute(attr);
}