; Test a bunch of cases where the cfg simplification code should
; be able to fold PHI nodes into computation in common cases.  Folding the PHI
; nodes away allows the branches to be eliminated, performing a simple form of
; 'if conversion'.

; RUN: opt < %s -simplifycfg -S > %t.xform
; RUN:   not grep phi %t.xform 
; RUN:   grep ret %t.xform

declare void @use(i1)

declare void @use.upgrd.1(i32)


define void @test(i1 %c, i32 %V, i32 %V2) {
; <label>:0
        br i1 %c, label %T, label %F
T:              ; preds = %0
        br label %F
F:              ; preds = %T, %0
        %B1 = phi i1 [ true, %0 ], [ false, %T ]                ; <i1> [#uses=1]
        %I6 = phi i32 [ %V, %0 ], [ 0, %T ]             ; <i32> [#uses=1]
        call void @use( i1 %B1 )
        call void @use.upgrd.1( i32 %I6 )
        ret void
}

