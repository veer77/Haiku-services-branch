/*
 * Copyright 2009, Ingo Weinhold, ingo_weinhold@gmx.de.
 * Distributed under the terms of the MIT License.
 */
#ifndef MAIN_MODEL_LOADER_H
#define MAIN_MODEL_LOADER_H


#include "AbstractModelLoader.h"
#include "Model.h"


class BDataIO;
class BDebugEventInputStream;
class DataSource;
struct system_profiler_thread_added;


class ModelLoader : public AbstractModelLoader {
public:
								ModelLoader(DataSource* dataSource,
									const BMessenger& target,
									void* targetCookie);

protected:
								~ModelLoader();

public:
			Model*				DetachModel();

protected:
	virtual	status_t			PrepareForLoading();
	virtual	status_t			Load();
	virtual	void				FinishLoading(bool success);

private:
			// shorthands for the longish structure names
			typedef system_profiler_thread_enqueued_in_run_queue
				thread_enqueued_in_run_queue;
			typedef system_profiler_thread_removed_from_run_queue
				thread_removed_from_run_queue;

private:
			status_t			_Load();
			status_t			_ReadDebugEvents(void** _eventData,
									size_t* _size);
			status_t			_ProcessEvent(uint32 event, uint32 cpu,
									const void* buffer, size_t size);

	inline	void				_UpdateLastEventTime(nanotime_t time);

			void				_HandleTeamAdded(
									system_profiler_team_added* event);
			void				_HandleTeamRemoved(
									system_profiler_team_removed* event);
			void				_HandleTeamExec(
									system_profiler_team_exec* event);
			void				_HandleThreadAdded(
									system_profiler_thread_added* event);
			void				_HandleThreadRemoved(
									system_profiler_thread_removed* event);
			void				_HandleThreadScheduled(
									system_profiler_thread_scheduled* event);
			void				_HandleThreadEnqueuedInRunQueue(
									thread_enqueued_in_run_queue* event);
			void				_HandleThreadRemovedFromRunQueue(
									thread_removed_from_run_queue* event);
			void				_HandleWaitObjectInfo(
									system_profiler_wait_object_info* event);

			Model::ThreadSchedulingState* _AddThread(
									system_profiler_thread_added* event);
			void				_AddThreadWaitObject(Model::ThreadSchedulingState* thread,
									uint32 type, addr_t object);

private:
			Model*				fModel;
			DataSource*			fDataSource;
			nanotime_t			fBaseTime;
			Model::SchedulingState fState;
};


#endif	// MAIN_MODEL_LOADER_H
