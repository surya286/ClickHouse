#pragma once

#include <Storages/MergeTree/MergeTreeBlockReadUtils.h>
#include <Storages/MergeTree/MergeTreeData.h>
#include <Storages/SelectQueryInfo.h>

#include <Processors/Sources/SourceWithProgress.h>


namespace DB
{

class IMergeTreeReader;
class UncompressedCache;
class MarkCache;
struct PrewhereExprInfo;

/// Base class for MergeTreeThreadSelectProcessor and MergeTreeSelectProcessor
class MergeTreeBaseSelectProcessor : public SourceWithProgress
{
public:
    MergeTreeBaseSelectProcessor(
        Block header,
        const MergeTreeData & storage_,
        const StorageMetadataPtr & metadata_snapshot_,
        const PrewhereInfoPtr & prewhere_info_,
        ExpressionActionsSettings actions_settings,
        UInt64 max_block_size_rows_,
        UInt64 preferred_block_size_bytes_,
        UInt64 preferred_max_column_in_block_size_bytes_,
        const MergeTreeReaderSettings & reader_settings_,
        bool use_uncompressed_cache_,
        const Names & virt_column_names_ = {});

    ~MergeTreeBaseSelectProcessor() override;

    static Block transformHeader(
        Block block, const PrewhereInfoPtr & prewhere_info, const DataTypePtr & partition_value_type, const Names & virtual_columns);

    static std::unique_ptr<MergeTreeBlockSizePredictor> getSizePredictor(
        const MergeTreeData::DataPartPtr & data_part,
        const MergeTreeReadTaskColumns & task_columns,
        const Block & sample_block);

protected:
    Chunk generate() final;

    /// Creates new this->task, and initializes readers.
    virtual bool getNewTask() = 0;

    virtual Chunk readFromPart();

    Chunk readFromPartImpl();

    /// Two versions for header and chunk.
    static void
    injectVirtualColumns(Block & block, MergeTreeReadTask * task, const DataTypePtr & partition_value_type, const Names & virtual_columns);
    static void
    injectVirtualColumns(Chunk & chunk, MergeTreeReadTask * task, const DataTypePtr & partition_value_type, const Names & virtual_columns);

    void initializeRangeReaders(MergeTreeReadTask & task);

protected:
    const MergeTreeData & storage;
    StorageMetadataPtr metadata_snapshot;

    PrewhereInfoPtr prewhere_info;
    std::unique_ptr<PrewhereExprInfo> prewhere_actions;

    UInt64 max_block_size_rows;
    UInt64 preferred_block_size_bytes;
    UInt64 preferred_max_column_in_block_size_bytes;

    MergeTreeReaderSettings reader_settings;

    bool use_uncompressed_cache;

    Names virt_column_names;

    DataTypePtr partition_value_type;

    /// This header is used for chunks from readFromPart().
    Block header_without_virtual_columns;

    std::unique_ptr<MergeTreeReadTask> task;

    std::shared_ptr<UncompressedCache> owned_uncompressed_cache;
    std::shared_ptr<MarkCache> owned_mark_cache;

    using MergeTreeReaderPtr = std::unique_ptr<IMergeTreeReader>;
    MergeTreeReaderPtr reader;
    MergeTreeReaderPtr pre_reader;
};

}
