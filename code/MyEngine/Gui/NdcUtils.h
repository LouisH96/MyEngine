#pragma once

namespace MyEngine
{
	namespace Gui
	{
		class NdcUtils
		{
		public:
			//---| More General |---
			static Float2 GetMouseNdc(const Float2& invCanvasSize);
			static Float2 ScreenPointToNdc(const Int2& point, const Float2& invCanvasSize);
			static Float2 ScreenPointToNdc(const Float2& point, const Float2& invCanvasSize);

			//---| Very specific |---
			//very specific screen to ndc stuff. Taking into account the pivot for alignment (same pivot for ndcRect & screen)
			static Float2 GetInvSize(const Int2& size);

			/**
			 * \brief used to update ndc rects when the canvas is resized
			 * \param newSize the new canvas size
			 * \param invCanvasSize the current invCanvasSize, will replace with new invCanvasSize
			 * \return the old to new scale, used to scale the actual rects
			 */
			static Float2 UpdateInvCanvasSize(const Int2& newSize, Float2& invCanvasSize);

			static void Resize(const Float2& scale, const Float2& pivot, Float2& rectCenter, Float2& rectSize);

			static void ScreenRectToNdc(const Float2& invCanvasSize, const Float2& offset, const Float2& size, const Float2& pivot, Float2& rectCenter, Float2& rectSize);

			static void SetScreenSpaceOffset(const Float2& invCanvasSize, const Float2& pivot, const Float2& screenSpaceCenter, const Float2& rectSize, Float2& center);
			static void SetScreenSpaceOffsetX(const Float2& invCanvasSize, const Float2& pivot, float screenSpaceX, const Float2& rectSize,  Float2& center);
			static void SetScreenSpaceOffsetY(const Float2& invCanvasSize, const Float2& pivot, float screenSpaceY, const Float2& rectSize,  Float2& center);
		};
	}
}
