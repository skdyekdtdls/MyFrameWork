
/*
0. ANIM_ENUM�� �ش��ϴ� �������� Cline_Enum.h�� �����Ѵ�.
1. StateMachine<OWNER, ANIM_ENUM>�� �δ��� Create�Ѵ�(���� ������ϵ� �����ϱ�).
2. ���ϴ� ����(������ Ȱ��)�� �����ϰ� Add_State�� StateMachine�� umap�� �ִ´�.
3. ���ӿ�����Ʈ�� typedef StateContext<OWNER, ANIM_ENUM> "X + State"�� �����Ѵ�.; 
4. ������Ʈ �߰� ������ ��ģ��.
	4-1. ���� ����
	4-2. Add_Component�� �߰�
	4-3. Tick�Լ� ȣ��
	4-4. Safe_Release
5. Initialize���� ������Ʈ�� TransitonTo()�� ȣ���Ѵ�.
*/

#pragma once
#include "StateMachine.h"
