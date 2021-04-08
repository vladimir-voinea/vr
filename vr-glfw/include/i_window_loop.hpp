#pragma once

namespace vr
{
	/**
	* @brief Callback interface for a window main loop
	*/
	class i_window_loop
	{
	public:

		/**
		* @brief Called in a loop by a window
		* \return Returns true/false to keep the window running or to close it respectively
		*/
		virtual bool loop() = 0;
	};
}