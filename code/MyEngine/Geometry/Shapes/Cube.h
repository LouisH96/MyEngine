#pragma once

namespace MyEngine
{
	class Cube
	{
	public:
		static constexpr unsigned NR_SIDES = 4;

		Cube() = default;
		explicit Cube(const Float3& position, const Float3& size);
		explicit Cube(const Transform& transform, const Float3& size);

		Transform& GetTransform() { return m_Transform; }
		Float3& GetPosition() { return m_Transform.Position; }
		Float3& GetSize() { return m_Size; }

		float GetWidth() const { return m_Size.x; }
		float GetHeight() const { return m_Size.y; }
		float GetDepth() const { return m_Size.z; }

		void SetPosition(const Float3& position) { m_Transform.Position = position; }
		void SetSize(const Float3& size) { m_Size = size; }

		const Transform& GetTransform() const { return m_Transform; }
		const Float3& GetPosition() const { return m_Transform.Position; }
		const Float3& GetSize() const { return m_Size; }

		Float3 GetRight() const { return m_Transform.Rotation.GetRight(); }
		Float3 GetUp() const { return m_Transform.Rotation.GetUp(); }
		Float3 GetForward() const { return m_Transform.Rotation.GetForward(); }

		Float3 GetRightBotBack() const;
		Float3 GetRightBotFront() const;
		Float3 GetLeftBotBack() const;
		Float3 GetLeftTopFront() const;
		Float3 GetCenter() const;

	private:
		Transform m_Transform;
		Float3 m_Size;
	};
}
