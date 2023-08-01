#include "pch.h"
#include "ConeCubeCollision.h"

#include "Geometry/Shapes/Cone.h"
#include "Geometry/Shapes/Cube.h"

using namespace Physics;

bool ConeCubeCollision::Detect(const Cone& cone, const Cube& cube)
{
	constexpr float debugLineLength{ 50 };

	//cube data
	const Float2 cubeLeftBot{ cube.GetPosition().Xz() };
	const Float2 cubeRightBot{ cube.GetRightBotFront().Xz() };
	const Float2 cubeForward{ cube.GetForward().Xz() };
	const Float2 cubeRight{ cube.GetRight().Xz() };
	const Float2 cubeLeftForward{ cube.GetLeftBotBack().Xz() };

	//debug cube lines
	const Float3 cubeLineColor{ 1,0,0 };
	const Float2 cubeLeftLineBack{ cubeLeftBot - cubeForward * debugLineLength };
	const Float2 cubeLeftLineFront(cubeLeftBot + cubeForward * (cube.GetSize().z + debugLineLength));
	const Float2 cubeRightLineBack{ cubeLeftLineBack + cubeRight * cube.GetSize().x };
	const Float2 cubeRightLineFront{ cubeLeftLineFront + cubeRight * cube.GetSize().x };
	const Float2 cubeBotLineLeft{ cubeLeftBot - cubeRight * debugLineLength };
	const Float2 cubeBotLineRight{ cubeLeftBot + cubeRight * debugLineLength };
	const Float2 cubeTopLineLeft{ cubeBotLineLeft + cubeForward * cube.GetSize().z };
	const Float2 cubeTopLineRight{ cubeBotLineRight + cubeForward * cube.GetSize().z };
	DebugRenderer::DrawLine(Float3::FromXz(cubeLeftLineBack), Float3::FromXz(cubeLeftLineFront), cubeLineColor);
	DebugRenderer::DrawLine(Float3::FromXz(cubeRightLineBack), Float3::FromXz(cubeRightLineFront), cubeLineColor);
	DebugRenderer::DrawLine(Float3::FromXz(cubeBotLineLeft), Float3::FromXz(cubeBotLineRight), cubeLineColor);
	DebugRenderer::DrawLine(Float3::FromXz(cubeTopLineLeft), Float3::FromXz(cubeTopLineRight), cubeLineColor);

	//cone data
	const Float2 coneOrigin{ cone.origin.Xz() };
	const Float2 coneForward{ cone.direction.Xz() };
	const float spreadAngle{ cone.GetAngle() };
	const float spreadCos{ cosf(spreadAngle) };
	const float spreadSin{ sinf(spreadAngle) };
	const Float2 side1Forward{
		spreadCos * coneForward.x - spreadSin * coneForward.y,
		spreadCos * coneForward.y + spreadSin * coneForward.x
	};
	const Float2 side2Forward{
		coneForward.x * spreadCos + coneForward.y * spreadSin,
		coneForward.y * spreadCos - coneForward.x * spreadSin
	};
	const Float2 side1Perpendicular{ -side1Forward.y, side1Forward.x };
	const Float2 side2Perpendicular{ -side2Forward.y, side2Forward.x };

	//draw cone lines
	const Float3 side1Ray{ Float3{side1Forward.x, 0, side1Forward.y} *debugLineLength };
	const Float3 side2Ray{ Float3{side2Forward.x, 0, side2Forward.y} *debugLineLength };
	DebugRenderer::DrawRay(cone.origin - side1Ray, side1Ray * 2, { 0,0,1 });
	DebugRenderer::DrawRay(cone.origin - side2Ray, side2Ray * 2, { 0,0,1 });

	//collision left
	const Float2 lineToCone{ coneOrigin - cubeLeftBot };
	float invSin1{ 1.f / (cubeForward.y * side1Forward.x - cubeForward.x * side1Forward.y) };
	float invSin2{ 1.f / (cubeForward.y * side2Forward.x - cubeForward.x * side2Forward.y) };
	float side1HitDistance{ lineToCone.Dot(side1Perpendicular) * invSin1 };
	float side2HitDistance{ lineToCone.Dot(side2Perpendicular) * invSin2 };

	Float2 edge1Hit{ cubeLeftBot + cubeForward * side1HitDistance };
	Float2 edge2Hit{ cubeLeftBot + cubeForward * side2HitDistance };
	DebugRenderer::DrawRay(Float3{ edge1Hit.x, 0, edge1Hit.y }, { 0,3,0 }, { 1,0,0 });
	DebugRenderer::DrawRay(Float3{ edge2Hit.x, 0, edge2Hit.y }, { 0,3,0 }, { 1,0,0 });

	//collision right
	side1HitDistance -= cubeRight.Dot(side1Perpendicular) * cube.GetSize().x * invSin1;
	side2HitDistance -= cubeRight.Dot(side2Perpendicular) * cube.GetSize().x * invSin2;
	edge1Hit = cubeRightBot + cubeForward * side1HitDistance;
	edge2Hit = cubeRightBot + cubeForward * side2HitDistance;
	DebugRenderer::DrawRay(Float3{ edge1Hit.x, 0, edge1Hit.y }, { 0,3,0 }, { 1,0,0 });
	DebugRenderer::DrawRay(Float3{ edge2Hit.x, 0, edge2Hit.y }, { 0,3,0 }, { 1,0,0 });

	//collision bot
	invSin1 = 1.f / (cubeRight.y * side1Forward.x - cubeRight.x * side1Forward.y);
	invSin2 = 1.f / (cubeRight.y * side2Forward.x - cubeRight.x * side2Forward.y);
	side1HitDistance = lineToCone.Dot(side1Perpendicular) * invSin1;
	side2HitDistance = lineToCone.Dot(side2Perpendicular) * invSin2;
	edge1Hit = cubeLeftBot + cubeRight * side1HitDistance;
	edge2Hit = cubeLeftBot + cubeRight * side2HitDistance;
	DebugRenderer::DrawRay(Float3{ edge1Hit.x, 0, edge1Hit.y }, { 0,3,0 }, { 1,0,0 });
	DebugRenderer::DrawRay(Float3{ edge2Hit.x, 0, edge2Hit.y }, { 0,3,0 }, { 1,0,0 });

	//collision top
	side1HitDistance -= cubeForward.Dot(side1Perpendicular) * cube.GetSize().z * invSin1;
	side2HitDistance -= cubeForward.Dot(side2Perpendicular) * cube.GetSize().z * invSin2;
	edge1Hit = cubeLeftForward + cubeRight * side1HitDistance;
	edge2Hit = cubeLeftForward + cubeRight * side2HitDistance;
	DebugRenderer::DrawRay(Float3{ edge1Hit.x, 0, edge1Hit.y }, { 0,3,0 }, { 1,0,0 });
	DebugRenderer::DrawRay(Float3{ edge2Hit.x, 0, edge2Hit.y }, { 0,3,0 }, { 1,0,0 });

	return false;
}
